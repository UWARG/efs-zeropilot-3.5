/**
 * @file PID.cpp
 * @authors Ian Frosst, Anthony Berbari, Camron Sabahi
 */

#include "../Inc/PID.hpp"
#include "constrain.h"


/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/

float PIDController::execute(ControlData _data) {
    setDesired(_data.desired);
    setActual(_data.actual);
    setActualRate(_data.actualRate);
    integral = getIntegral();


    if (std::isnan(_data.desired) || std::isnan(_data.actual)) {
        return 0;
    }

    float error = _data.desired - _data.actual;
    float derivative;

    // avoid integral windup
    float error_change = (error - prevError);
    integral = constrain<float>(integral + error + error_change, pid.i_max, -pid.i_max);

    // integral = constrain<float>(integral + error, pid.i_max, -pid.i_max);

    // if we are provided with a measured derivative (say from a gyroscope), it
    // is always less noisy to use that than to compute it ourselves.
    if (!std::isnan(pid.actualRate)) {
        derivative = pid.actualRate * pid.kd;
    } else {

        derivative = execute_d_back(_data);
        //One is uncommented to test the difference between the two derivative calculations
        // derivative = execute_d_hist(_data);
    }

    float ret = constrain<float>((pid.kp * error) + (pid.ki * integral) - (derivative),
                                    pid.max_output, pid.min_output);

    //map pid output (angle/rate) to motor percentage
    ret = map(ret, pid.min_output, pid.max_output, 0, 100);

    prevError = error;

    return ret;
}

float PIDController::execute_p(ControlData _data) {
    float error = _data.desired - _data.actual;
    return pid.kp * error;
}

float PIDController::execute_i(ControlData _data) {
    float error = _data.desired - _data.actual;
    float error_change = (error - prevError);
    integral = constrain<float>(integral + error + error_change, pid.i_max, -pid.i_max);
    prevError = error;
    return pid.ki * integral;
}

float PIDController::execute_d_hist(ControlData _data) {
    float derivative;

    if (!std::isnan(_data.actualRate)) {
        derivative = _data.actualRate;
    } else {
        historicalValue[2] = historicalValue[1];
        historicalValue[1] = historicalValue[0];
        historicalValue[0] = pid.actual;

        derivative = ((3 * historicalValue[0]) - (4 * historicalValue[1]) + (historicalValue[2]));
    }

    return pid.kd * derivative;
}

float PIDController::execute_d_back(ControlData _data) {
    float error = _data.desired - _data.actual;
    float derivative = error - prevError;
    prevError = error;
    return pid.kd * derivative;
}

void PIDController::updatePIDGains(Gains _gains) {
    pid.kp = _gains.kp;
    pid.ki = _gains.ki;
    pid.kd = _gains.kd;
}

float PIDController:: map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// void PIDController::updatePidFixedFreq( PIDValues values, TickType_t updateFreq, volatile bool& stopFlag) {
    
//     pid.desired = values.desired;
//     pid.actual = values.actual;
//     pid.actualRate = values.actualRate;

//     TickType_t xLastWakeTime;
//     const TickType_t xFrequency = updateFreq;

//     xLastWakeTime = xTaskGetTickCount();

//     while (!stopFlag) {
//         execute();

//         vTaskDelayUntil(&xLastWakeTime, xFrequency);
//     }
// }
    
// void PIDController::periodicTask(void* pvParameters) {
//     PIDController* pid = static_cast<PIDController*>(pvParameters);

//     TickType_t updateFrequency = pdMS_TO_TICKS(pid->getTaskUpdateFrequency());
//     TickType_t lastWakeTime = xTaskGetTickCount();

//     while (!pid->getStopFlag()){
//         float result = pid->execute();
//         vTaskDelayUntil(&lastWakeTime, updateFrequency);
//     }
//     vTaskDelete(NULL);
// }

// void PIDController::createPeriodicTask(uint32_t updateFrequency) {

//     setTaskUpdateFrequency(updateFrequency); // = updateFrequency;
//     setStopFlag(false); //stopTaskFlag = false;
//     xTaskCreate(periodicTask, "PID Task", configMINIMAL_STACK_SIZE, this, 1, NULL);
// }

// void PIDController::stopUpdateTask() {
//     setStopFlag(true);
//     vTaskDelete(periodicTaskHandle);
// }