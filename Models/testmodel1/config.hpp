#ifndef ZPSW3_CONFIG_HPP
#define ZPSW3_CONFIG_HPP

#include "global_config.hpp"
#include "tim.h"

namespace config
{

    /* Motor config */

    constexpr Motor_t motors[] = {
        {   //Yaw servo motor
            .axis = yaw,
            .isInverted = false,
            .driverConstructor = constructDriver<MotorChannel, PWMChannel,
                                                 /*timer*/ &htim1, /*timer_channel*/ 0>
        },
        {   //Roll BLDC motor
            .axis = roll,
            .isInverted = true,
            .driverConstructor = constructDriver<MotorChannel, TempDSHOTDriver>
        }
    };

    constexpr uint8_t NUM_MOTORS = sizeof(motors)/sizeof(Motor_t);



    /* RC input config */

    //Example array for a model that supports both PPM and SBUS input
    constexpr RCInput_t RCInputs[] = {
        {   //PPM input
            .driverConstructor = constructDriver<RCInputDriver, TempPPMDriver>
        },
        {   //SBUS input
            .driverConstructor = constructDriver<RCInputDriver, TempSBusDriver>
        }
    };
    
    constexpr uint8_t NUM_RC_INPUTS = sizeof(RCInputs)/sizeof(RCInput_t);



    /* GPS config */

    //Example array for a model with two GPS modules
    constexpr GPS_t GPSArray[] = {
        {   //NEOM8
            .driverConstructor = constructDriver<GPSDriver, TempNEOM8Driver>
        },
        {   //Other GPS
            .driverConstructor = constructDriver<GPSDriver, otherGPSDriver>
        }
    };
    
    constexpr uint8_t NUM_GPS = sizeof(GPSArray)/sizeof(GPS_t);



    /* Flightmode Config */

    class TempFlightmode1 : public AM::Flightmode{
        volatile uint8_t asdf_;
        public:
        TempFlightmode1(){}
        //TODO: Implement control algorithm functions in AM
        void run();
        void updatePid();
    };

    class TempFlightmode2 : public AM::Flightmode{
        public:
        TempFlightmode2(){}
        void run();
        void updatePid();
    };

    constexpr Flightmode_t flightmodes[] = {
        {
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
            .flightmodeConstructor = constructFlightmode<TempFlightmode1>
        },
        {
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
            .flightmodeConstructor = constructFlightmode<TempFlightmode2>
        }
    };

}

#endif // ZPSW3_GLOBAL_CONFIG_HPP