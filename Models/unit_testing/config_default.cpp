#include "config.hpp"

namespace config
{

    /* Motor config */

    Motor_t *motors = nullptr;
    
    uint8_t NUM_MOTORS = 0;



    /* RC input config */

    //Example array for a model that supports both PPM and SBUS input
    RCInput_t RCInputs[] = {
        {   //PPM input
            .driverConstructor = constructObject<RCInputDriver, TempPPMDriver>
        },
        {   //SBUS input
            .driverConstructor = constructObject<RCInputDriver, TempSBusDriver>
        }
    };
    
    uint8_t NUM_RC_INPUTS = sizeof(RCInputs)/sizeof(RCInput_t);



    /* GPS config */

    //Example array for a model with two GPS modules
    GPS_t GPSArray[] = {
        {   //NEOM8
            .driverConstructor = constructObject<GPSDriver, TempNEOM8Driver>
        },
        {   //Other GPS
            .driverConstructor = constructObject<GPSDriver, otherGPSDriver>
        }
    };
    
    uint8_t NUM_GPS = sizeof(GPSArray)/sizeof(GPS_t);



    /* Flightmode Config */

    Flightmode_t flightmodes[] = {
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
