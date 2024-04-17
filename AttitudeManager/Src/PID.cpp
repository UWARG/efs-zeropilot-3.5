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
    if (std::isnan(_desired) || std::isnan(_actual)) {
        return 0;
    }

    setDesired(_desired);
    setActual(_actual);
    setActualRate(_actualRate);
    float integral = getIntegral();

    float error = _desired - _actual;
    float derivative;

    float error_change = (error - prevError);
    integral = constrain<float>(integral + error + error_change, pid_conf.i_max, -pid_conf.i_max);

    if (!std::isnan(pid_conf.actualRate)) {
        derivative = pid_conf.actualRate * pid_conf.kd;
    } else {
        // If cannot pull actualRate from IMU filter, use one of the derivative calculation with
        // back calculation
        derivative = error - prevError;
        prevError = error;

        // Alt method, use historical data to calculate derivative
        //  historicalValue[2] = historicalValue[1];
        //  historicalValue[1] = historicalValue[0];
        //  historicalValue[0] = _actual;
        //  derivative = ((3 * historicalValue[0]) - (4*historicalValue[1]) + (historicalValue[2]));
    }

    float ret = constrain<float>(
        (pid_conf.kp * error) + (pid_conf.ki * integral) - (pid_conf.kd * derivative),
        pid_conf.max_output, pid_conf.min_output);

    ret = map(ret, pid_conf.min_output, pid_conf.max_output);

    prevError = error;

    return ret;
}

float PIDController::execute_p(float _desired, float _actual, float _actualRate) {
    float error = _desired - _actual;
    return pid_conf.kp * error;
}

float PIDController::execute_i(float _desired, float _actual, float _actualRate) {
    float error = _desired - _actual;
    float error_change = (error - prevError);
    float integral = getIntegral();
    integral = constrain<float>(integral + error + error_change, pid_conf.i_max, -pid_conf.i_max);
    prevError = error;
    return pid_conf.ki * integral;
}

float PIDController::execute_d_hist(float _desired, float _actual, float _actualRate) {
    float derivative;

    historicalValue[2] = historicalValue[1];
    historicalValue[1] = historicalValue[0];
    historicalValue[0] = _actual;

    derivative = ((3 * historicalValue[0]) - (4 * historicalValue[1]) + (historicalValue[2]));

    return pid_conf.kd * derivative;
}

float PIDController::execute_d_back(float _desired, float _actual, float _actualRate) {
    float error = _desired - _actual;
    float derivative = error - prevError;
    prevError = error;
    return pid_conf.kd * derivative;
}

float PIDController::map(float x, float out_min, float out_max) {
    return (x - pid_conf.min_input) * (out_max - out_min) /
               (pid_conf.max_input - pid_conf.min_input) +
           out_min;
}

void PIDController::setGainTerm(GainTerm _gainTerm, float _desired_gain) {
    if (std::isnan(_desired_gain) || _desired_gain < 0 || _desired_gain > 1) {
        return;
    }

    switch (_gainTerm) {
        case Proportional:
            pid_conf.kp = _desired_gain;
            break;
        case Derivative:
            pid_conf.kd = _desired_gain;
            break;
        case Integral:
            pid_conf.ki = _desired_gain;
            break;
    }
}
