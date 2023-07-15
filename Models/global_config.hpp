#ifndef ZPSW3_GLOBAL_CONFIG_HPP
#define ZPSW3_GLOBAL_CONFIG_HPP

#include <cstdint>

#include "AM_ControlAlgorithm.hpp"
#include "ZP_D_PWMChannel.hpp"

namespace config
{
    using percentage_t = float;

        class TempDSHOTDriver : public MotorChannel
        {
            public:
                TempDSHOTDriver() {}
                void set(uint8_t percent){}
        };
    class RCInputDriver{};
        class TempPPMDriver : public RCInputDriver{};
        class TempSBusDriver : public RCInputDriver{};
    class UnifiedSensorDriver{};
        class TempVN300Driver : public UnifiedSensorDriver{};
        class TempVN310Driver : public UnifiedSensorDriver{};
    class GPSDriver{};
        class TempNEOM8Driver : public GPSDriver{};
        class otherGPSDriver : public GPSDriver{};
    class IMUDriver{};
        class TempMPU6050Driver : public IMUDriver{};
    class AirspeedDriver{};
        class someAirspeedDriver : public AirspeedDriver{};
     
    //Define factory function to create drivers
    template <class BaseDriverType, class DerivedDriverType, auto... args>
    BaseDriverType* constructDriver() {
        return new DerivedDriverType(args...);
    }

    //Typedef pointer to factory function
    template <class BaseDriverType>
    using DriverFactory = BaseDriverType* (*)(void);



    /* Motor declarations */

    typedef enum {
        yaw,
        pitch,
        roll,
        thrust
    } ControlAxis_t;

    typedef struct {
        ControlAxis_t axis;
        bool isInverted = false;
        DriverFactory<MotorChannel> driverConstructor;
    } Motor_t;



    /* RC input declarations */

    typedef struct {
        //TODO: determine other config fields relevant to the RC input type
        DriverFactory<RCInputDriver> driverConstructor;
    } RCInput_t;



    /* GPS declarations */

    typedef struct {
        DriverFactory<GPSDriver> driverConstructor;
    } GPS_t;



    /* Flightmode declarations */

    //Define factory function to create flightmodes
    template <class FlightmodeType, auto... args>
    AM::Flightmode* constructFlightmode() {
        return new FlightmodeType(args...);
    }

    //Typedef pointer to factory function
    using FlightmodeFactory = AM::Flightmode* (*)(void);

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
        AxisPID_t thrustPID = {};
    } ControlPID_t;

    typedef struct {
        percentage_t min = 0.0f;
        percentage_t max = 100.0f;
    } AxisLimits_t;

    typedef struct {
        AxisLimits_t yawLimit = {};
        AxisLimits_t pitchLimit = {};
        AxisLimits_t rollLimit = {};
        AxisLimits_t thrustLimit = {};
    } ControlLimits_t;

    typedef struct {
        ControlPID_t PIDValues = {};
        ControlLimits_t controlLimits = {};
    } ControlTuning_t;
    
    typedef struct {
        ControlTuning_t tuningData;
        FlightmodeFactory flightmodeConstructor;
    } Flightmode_t;

}

#endif // ZPSW3_GLOBAL_CONFIG_HPP