#include "config.hpp"

//TODO: Consider where these should be initialized
TempPWMDriver PWMDriver;
TempDSHOTDriver DSHOTdriver;

Motor_t motors[] = {
    {   //Yaw servo motor
        .axis = yaw,
        .direction = positive,
        .driver = PWMDriver,
        .interfaceID = 0
    },
    {   //Roll BLDC motor
        .axis = roll,
        .direction = negative,
        .driver = DSHOTdriver,
        .interfaceID = 0
    }
};

const uint8_t NUM_MOTORS = sizeof(motors)/sizeof(Motor_t);

PID_t flightmode1Tuning = {
    .p = 1.0f,
    .i = 1.0f,
    .d = 1.0f
};

ControlLimits_t flightmode1Limits = {
    .yawLimit = {
        .min = 0.0f,
        .max = 100.0f
    },
    .pitchLimit = {
        .min = 0.0f,
        .max = 100.0f
    },
    .rollLimit = {
        .min = 0.0f,
        .max = 100.0f
    },
    .thrustLimit = {
        .min = 0.0f,
        .max = 100.0f
    }
};

//TODO: Initialize flightmode subclass members