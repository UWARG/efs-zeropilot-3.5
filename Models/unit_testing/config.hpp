#ifndef ZPSW3_CONFIG_HPP
#define ZPSW3_CONFIG_HPP

#include "config_foundation.hpp"

namespace config
{

    /* Motor config */

    extern Motor_t *motors;

    extern uint8_t NUM_MOTORS;



    /* RC input config */

    extern RCInput_t RCInputs[2];
    
    extern uint8_t NUM_RC_INPUTS;



    /* GPS config */

    extern GPS_t GPSArray[2];
    
    extern uint8_t NUM_GPS;



    /* Flightmode Config */

    extern Flightmode_t flightmodes[2];

}

#endif // ZPSW3_CONFIG_HPP