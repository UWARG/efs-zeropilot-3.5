#ifndef ZPSW3_CONFIG_FOUNDATION_HPP
#define ZPSW3_CONFIG_FOUNDATION_HPP

#include <cstdint>

#include "AM_ControlAlgorithm.hpp"
#include "temp_drivers.hpp"

namespace config
{
    using percentage_t = float;
     
    //Define factory function to create drivers and flightmodes
    template <class BaseClass, class DerivedClass, auto... args>
    BaseClass* constructObject() {
        return new DerivedClass(args...);
    }

    //Typedef pointer to factory function
    template <class BaseClass>
    using ObjectFactory = BaseClass* (*)(void);



    /* Motor declarations */

    typedef enum {
        yaw,
        pitch,
        roll,
        throttle,
        NUM_CONTROL_AXIS
    } ControlAxis_t;

    typedef struct {
        ControlAxis_t axis;
        bool isInverted = false;
        ObjectFactory<MotorChannel> driverConstructor;
    } Motor_t;



    /* RC input declarations */

    typedef struct {
        //TODO: determine other config fields relevant to the RC input type
        ObjectFactory<RCInputDriver> driverConstructor;
    } RCInput_t;



    /* GPS declarations */

    typedef struct {
        ObjectFactory<GPSDriver> driverConstructor;
    } GPS_t;



    /* Flightmode declarations */

    typedef struct {
        bool isEnabled = false;
        float p = 0.0f;
        float i = 0.0f;
        float d = 0.0f;
    } AxisPID_t;

    typedef struct {
        AxisPID_t yawPID = {};
        AxisPID_t pitchPID = {};
        AxisPID_t rollPID = {};
        AxisPID_t throttlePID = {};
    } ControlPID_t;

    typedef struct {
        percentage_t min = 0.0f;
        percentage_t max = 100.0f;
    } AxisLimits_t;

    typedef struct {
        AxisLimits_t yawLimit = {};
        AxisLimits_t pitchLimit = {};
        AxisLimits_t rollLimit = {};
        AxisLimits_t throttleLimit = {};
    } ControlLimits_t;

    typedef struct {
        ControlPID_t PIDValues = {};
        ControlLimits_t controlLimits = {};
    } ControlTuning_t;
    
    typedef struct {
        ControlTuning_t tuningData;
        ObjectFactory<AM::Flightmode> flightmodeConstructor;
    } Flightmode_t;

}

#endif // ZPSW3_CONFIG_FOUNDATION_HPP