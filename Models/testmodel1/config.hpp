#ifndef ZPSW3_CONFIG_HPP
#define ZPSW3_CONFIG_HPP

#include "global_config.hpp"

namespace config
{

    /* Motor Config */

    constexpr Motor_t motors[] = {
        {   //Yaw servo motor
            .axis = yaw,
            .isInverted = false,
            .driverConstructor = constructDriver<TempPWMDriver>,
            .interfaceID = 0
        },
        {   //Roll BLDC motor
            .axis = roll,
            .isInverted = true,
            .driverConstructor = constructDriver<TempDSHOTDriver>,
            .interfaceID = 0
        }
    };

    constexpr uint8_t NUM_MOTORS = sizeof(motors)/sizeof(Motor_t);



    /* Flightmode 1 Config */

    constexpr ControlPID_t flightmode1Tuning = {
        .yawPID = {
            .isEnabled = false,
            .p = 0.0f,
            .i = 0.0f,
            .d = 0.0f
        },
        .pitchPID = {
            .isEnabled = false,
            .p = 0.0f,
            .i = 0.0f,
            .d = 0.0f
        },
        .rollPID = {
            .isEnabled = false,
            .p = 0.0f,
            .i = 0.0f,
            .d = 0.0f
        },
        .thrustPID = {
            .isEnabled = false,
            .p = 0.0f,
            .i = 0.0f,
            .d = 0.0f
        },
    };

    constexpr ControlLimits_t flightmode1Limits = {
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

    constexpr Flightmode TempFlightmode1 = Flightmode(flightmode1Tuning, flightmode1Limits);



    /* Flightmode 2 Config */

    constexpr ControlPID_t flightmode2Tuning = {
        .yawPID = {
            .isEnabled = true,
            .p = 1.0f,
            .i = 1.0f,
            .d = 1.0f
        },
        .pitchPID = {
            .isEnabled = false,
            .p = 0.0f,
            .i = 0.0f,
            .d = 0.0f
        },
        .rollPID = {
            .isEnabled = true,
            .p = 1.0f,
            .i = 1.0f,
            .d = 1.0f
        },
        .thrustPID = {
            .isEnabled = false,
            .p = 0.0f,
            .i = 0.0f,
            .d = 0.0f
        },
    };

    constexpr ControlLimits_t flightmode2Limits = {
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
    
    constexpr Flightmode TempFlightmode2 = Flightmode(flightmode2Tuning, flightmode2Limits);

}

#endif // ZPSW3_GLOBAL_CONFIG_HPP