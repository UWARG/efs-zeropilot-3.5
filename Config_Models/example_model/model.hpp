#ifndef ZPSW3_CONFIG_HPP
#define ZPSW3_CONFIG_HPP

#include "config_foundation.hpp"
#include "manual.hpp"
#include "fbwa.hpp"
#include "tim.h"
#include "ZP_D_PWMChannel.hpp"

namespace config
{

    /* Motor config */

    constexpr Motor_t motors[] = {
        {   //Yaw servo motor
            .axis = yaw,
            .isInverted = false,
            .driverConstructor = constructObject<MotorChannel, PWMChannel,
                                                 /*timer*/ &htim1, /*timer_channel*/ 0>
        },
        {   //Roll BLDC motor
            .axis = roll,
            .isInverted = true,
            .driverConstructor = constructObject<MotorChannel, TempDSHOTDriver>
        }
    };

    constexpr uint8_t NUM_MOTORS = sizeof(motors)/sizeof(Motor_t);



    /* RC input config */

    //Example array for a model that supports both PPM and SBUS input
    constexpr RCInput_t RCInputs[] = {
        {   //PPM input
            .driverConstructor = constructObject<RCInputDriver, TempPPMDriver>
        },
        {   //SBUS input
            .driverConstructor = constructObject<RCInputDriver, TempSBusDriver>
        }
    };

    constexpr uint8_t NUM_RC_INPUTS = sizeof(RCInputs)/sizeof(RCInput_t);



    /* GPS config */

    //Example array for a model with two GPS modules
    constexpr GPS_t GPSArray[] = {
        {   //NEOM8
            .driverConstructor = constructObject<GPSDriver, TempNEOM8Driver>
        },
        {   //Other GPS
            .driverConstructor = constructObject<GPSDriver, otherGPSDriver>
        }
    };

    constexpr uint8_t NUM_GPS = sizeof(GPSArray)/sizeof(GPS_t);



    /* Flightmode Config */

    constexpr Flightmode_t flightmodes[] = {
        {   //Flightmode1
            .tuningData{
                .PIDValues = {
                    .yawPID = {
                        .isEnabled = true,
                        .p = 1.0f,
                        .i = 1.0f,
                        .d = 1.0f
                    }
                },
                .controlLimits = {
                    .yawLimit = {
                        .min = 5.0f,
                        .max = 95.0f
                    }
                }
            },
            .flightmodeConstructor = constructObject<AM::Flightmode, AM::Manual>
        },
        {   //Flightmode2
            .tuningData{
                .PIDValues = {
                    .yawPID = {
                        .isEnabled = true,
                        .p = 1.0f,
                        .i = 1.0f,
                        .d = 1.0f
                    },
                    .pitchPID = {
                        .isEnabled = true,
                        .p = 1.0f,
                        .i = 1.0f,
                        .d = 1.0f
                    },
                    .rollPID = {
                        .isEnabled = true,
                        .p = 1.0f,
                        .i = 1.0f,
                        .d = 1.0f
                    },
                    .throttlePID = {
                        .isEnabled = true,
                        .p = 1.0f,
                        .i = 1.0f,
                        .d = 1.0f
                    }
                },
                .controlLimits = {
                    .yawLimit = {
                        .min = -95.0f,
                        .max = 95.0f
                    },
                    .pitchLimit = {
                        .min = -30.0f,
                        .max = 30.0f
                    },
                    .rollLimit = {
                        .min = -100.0f,
                        .max = 100.0f
                    },
                    .throttleLimit = {
                        .min = 0.0f,
                        .max = 100.0f
                    },
                }
            },
            .flightmodeConstructor = constructObject<AM::Flightmode, AM::FBWA>
        }
    };

}

#endif // ZPSW3_CONFIG_HPP