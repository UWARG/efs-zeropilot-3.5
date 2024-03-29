/**
 * @file PID.cpp
 * @authors Ian Frosst, Anthony Berbari
 */

#include "PID.hpp"

#include "constrain.h"

/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/

float PIDController::execute(ControlData _data) {
    if (std::isnan(_data.desired) || std::isnan(_data.actual)){
        return 0;
    }

    setDesired(_data.desired);
    setActual(_data.actual);
    setActualRate(_data.actualRate);
    float integral = getIntegral();

    float error = _data.desired - _data.actual;
    float derivative;

    float error_change = (error - prevError);
    integral = constrain<float>(integral + error + error_change, pid.i_max, -pid.i_max);

    if (!std::isnan(pid.actualRate)){
        derivative = pid.actualRate * pid.kd;
    } else {
        derivative = execute_d_back(_data);
        //One is uncommented to test diff between types of calculation
        // derivative = execute_d_hist(_data);
    }

    float ret = constrain<float>((pid.kp * error) + (pid.ki * integral) - (derivative),
                                pid.max_output, pid.min_output);

    ret = map(ret, pid.min_output, pid.max_output, 0, 100);

    prevError = error;

    return ret;
}

float PIDController::execute_p(ControlData _data){
    float error = _data.desired - _data.actual;
    return pid.kp * error;
}

float PIDController::execute_i(ControlData _data) {
    float error = _data.desired - _data.actual;
    float error_change = (error - prevError);
    float integral = getIntegral();
    integral = constrain<float>(integral + error + error_change, pid.i_max, -pid.i_max);
    prevError = error;
    return pid.ki * integral;
}

float PIDController::execute_d_hist(ControlData _data) {
    float derivative;

    historicalValue[2] = historicalValue[1];
    historicalValue[1] = historicalValue[0];
    historicalValue[0] = _data.actual;

    derivative = ((3 * historicalValue[0]) - (4*historicalValue[1]) + (historicalValue[2]));

    return pid.kd * derivative;
}

float PIDController::execute_d_back(ControlData _data){
    float error = _data.desired - _data.actual;
    float derivative = error - prevError;
    prevError = error;
    return pid.kd * derivative;
}

void PIDController::updatePIDGains(PIDGains _gains) {
    pid.kp = _gains.kp;
    pid.ki = _gains.ki;
    pid.kd = _gains.kd;
}

float PIDController::map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}