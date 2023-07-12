#ifndef ZPSW3_CONFIG_HPP
#define ZPSW3_CONFIG_HPP

#include "global_config.hpp"

namespace config
{

    /* Motor config */

    constexpr Motor_t motors[] = {
        {   //Yaw servo motor
            .axis = yaw,
            .isInverted = false,
            .driverConstructor = constructDriver<MotorDriver, TempPWMDriver,
                                                 /*timer*/ 0, /*timer_channel*/ 0>
        },
        {   //Roll BLDC motor
            .axis = roll,
            .isInverted = true,
            .driverConstructor = constructDriver<MotorDriver, TempDSHOTDriver>
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



    /* Flightmode 1 Config */

    constexpr ControlTuning_t flightmode1Tuning = {
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
    };

    class TempFlightmode1 : public Flightmode{
        public:
        constexpr TempFlightmode1() : Flightmode(flightmode1Tuning){}
        //TODO: Implement control algorithm functions in AM
        void run();
        void updatePid();
    };

    static TempFlightmode1 tempFlightmode1;



    /* Flightmode 2 Config */

    constexpr ControlTuning_t flightmode2Tuning = {
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
    };
    
    class TempFlightmode2 : public Flightmode{
        public:
        constexpr TempFlightmode2() : Flightmode(flightmode2Tuning){}
        void run();
        void updatePid();
    };
    
    static TempFlightmode2 tempFlightmode2;

    constexpr Flightmode * flightmodes[] = {
        &tempFlightmode1,
        &tempFlightmode2
    };

}

#endif // ZPSW3_GLOBAL_CONFIG_HPP