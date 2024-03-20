/**
 * @file PID.cpp
 * @authors Ian Frosst, Anthony Berbari, Camron Sabahi
 */

#include "../Inc/PID.hpp"
#include "constrain.h"

// #include "FreeRTOS.h"
// #include "task.h"

/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/

float PIDController::execute() {
    if (std::isnan(pid.desired) || std::isnan(pid.actual)) {
        return 0;
    }

    float error = pid.desired - pid.actual;
    float derivative;

    // avoid integral windup
    integral = constrain<float>(integral + error, pid.i_max, -pid.i_max);

    // if we are provided with a measured derivative (say from a gyroscope), it
    // is always less noisy to use that than to compute it ourselves.
    if (!std::isnan(pid.actualRate)) {
        derivative = pid.actualRate;
    } else {
        historicalValue[2] = historicalValue[1];
        historicalValue[1] = historicalValue[0];
        historicalValue[0] = pid.actual;

        // Finite difference approximation gets rid of noise much better than
        // first order derivative computation
        derivative = ((3 * historicalValue[0]) - (4 * historicalValue[1]) + (historicalValue[2]));
    }

    float ret = constrain<float>((pid.kp * error) + (pid.ki * integral) - (pid.kd * derivative),
                                    pid.max_output, pid.min_output);

    //map pid output (angle/rate) to motor percentage
    ret = map(ret, pid.min_output, pid.max_output, 0, 100);

    return ret;
}

float PIDController::execute_p() {
    float error = pid.desired - pid.actual;
    return pid.kp * error;
}

float PIDController::execute_i() {
    float error = pid.desired - pid.actual;
    integral = constrain<float>(integral + error, pid.i_max, -pid.i_max);
    return pid.ki * integral;
}

float PIDController::execute_d() {
    float derivative;

    if (!std::isnan(pid.actualRate)) {
        derivative = pid.actualRate;
    } else {
        historicalValue[2] = historicalValue[1];
        historicalValue[1] = historicalValue[0];
        historicalValue[0] = pid.actual;

        derivative = ((3 * historicalValue[0]) - (4 * historicalValue[1]) + (historicalValue[2]));
    }

    return pid.kd * derivative;
}

void PIDController::updatePIDGains(float _kp, float _ki, float _kd) {
    pid.kp = _kp;
    pid.ki = _ki;
    pid.kd = _kd;
}

float PIDController:: map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void PIDController::updatePidFixedFreq( PIDValues values, TickType_t updateFreq, volatile bool& stopFlag) {
    
    pid.desired = values.desired;
    pid.actual = values.actual;
    pid.actualRate = values.actualRate;

    TickType_t xLastWakeTime;
    const TickType_t xFrequency = updateFreq;

    xLastWakeTime = xTaskGetTickCount();

    while (!stopFlag) {
        execute();

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
    
void PIDController::periodicTask(void* pvParameters) {
    PIDController* pid = static_cast<PIDController*>(pvParameters);

    TickType_t updateFrequency = pdMS_TO_TICKS(pid->getTaskUpdateFrequency());
    TickType_t lastWakeTime = xTaskGetTickCount();

    while (!pid->getStopFlag()){
        float result = pid->execute();
        vTaskDelayUntil(&lastWakeTime, updateFrequency);
    }
    vTaskDelete(NULL);
}

void PIDController::createPeriodicTask(uint32_t updateFrequency) {

    setTaskUpdateFrequency(updateFrequency); // = updateFrequency;
    setStopFlag(false); //stopTaskFlag = false;
    xTaskCreate(periodicTask, "PID Task", configMINIMAL_STACK_SIZE, this, 1, NULL);
}

void PIDController::stopUpdateTask() {
    setStopFlag(true);
    vTaskDelete(periodicTaskHandle);
}