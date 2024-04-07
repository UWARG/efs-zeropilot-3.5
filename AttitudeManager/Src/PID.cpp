/**
 * @file PID.cpp
 * @authors Ian Frosst, Anthony Berbari
 */

#include "PID.hpp"

#include "constrain.h"

/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/

float PIDController::execute(float _desired, float _actual, float _actualRate) {
    if (std::isnan(_desired) || std::isnan(_actual)){
        return 0;
    }

    setDesired(_desired);
    setActual(_actual);
    setActualRate(_actualRate);
    float integral = getIntegral();

    float error = _desired - _actual;
    float derivative;

    float error_change = (error - prevError);
    integral = constrain<float>(integral + error + error_change, pid.i_max, -pid.i_max);

    if (!std::isnan(pid.actualRate)){
        derivative = pid.actualRate * pid.kd;
    } else {
        derivative = execute_d_back(_desired, _actual, _actualRate);
        //One is uncommented to test diff between types of calculation
        // derivative = execute_d_hist(_desired, _actual, _actualRate);
    }

    float ret = constrain<float>((pid.kp * error) + (pid.ki * integral) - (derivative),
                                pid.max_output, pid.min_output);

    ret = map(ret, pid.min_output, pid.max_output, 0, 100);

    prevError = error;

    return ret;
}

float PIDController::execute_p(float _desired, float _actual, float _actualRate){
    float error = _desired - _actual;
    return pid.kp * error;
}

float PIDController::execute_i(float _desired, float _actual, float _actualRate) {
    float error = _desired - _actual;
    float error_change = (error - prevError);
    float integral = getIntegral();
    integral = constrain<float>(integral + error + error_change, pid.i_max, -pid.i_max);
    prevError = error;
    return pid.ki * integral;
}

float PIDController::execute_d_hist(float _desired, float _actual, float _actualRate) {
    float derivative;

    historicalValue[2] = historicalValue[1];
    historicalValue[1] = historicalValue[0];
    historicalValue[0] = _actual;

    derivative = ((3 * historicalValue[0]) - (4*historicalValue[1]) + (historicalValue[2]));

    return pid.kd * derivative;
}

float PIDController::execute_d_back(float _desired, float _actual, float _actualRate){
    float error = _desired - _actual;
    float derivative = error - prevError;
    prevError = error;
    return pid.kd * derivative;
}

float PIDController::map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}