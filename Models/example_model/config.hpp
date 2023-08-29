#ifndef ZPSW3_CONFIG_HPP
#define ZPSW3_CONFIG_HPP

#include "config_foundation.hpp"
#include "tim.h"

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
            .flightmodeConstructor = constructObject<AM::Flightmode, ExampleFlightmode1>
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
                    .rollPID = {
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
                    },
                    .rollLimit = {
                        .min = 0.0f,
                        .max = 100.0f
                    }
                }
            },
            .flightmodeConstructor = constructObject<AM::Flightmode, ExampleFlightmode2>
        }
    };

}

#endif // ZPSW3_CONFIG_HPP