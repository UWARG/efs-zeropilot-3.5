#ifndef ZPSW3_GLOBAL_CONFIG_HPP
#define ZPSW3_GLOBAL_CONFIG_HPP

#include <cstdint>
#include <functional>

namespace config
{

    //TODO: Include drivers to replace all of these temporary definitions
    class MotorDriver{};
        class TempPWMDriver : public MotorDriver
        {
            public:
                TempPWMDriver(uint8_t timer, uint16_t timer_channel)
                : timer_(timer), timerChannel_(timer_channel) {}
            private:
                uint8_t timer_;
                uint16_t timerChannel_;
        };
        class TempDSHOTDriver : public MotorDriver
        {
            public:
                TempDSHOTDriver() {}
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
     
    //Define factory function to create motor drivers.
    //The driver class must be copy-constructable as the returned object will be a copy.
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
        DriverFactory<MotorDriver> driverConstructor;
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
        float min = 0.0f;
        float max = 100.0f;
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
    

    //TODO: Combine flightmode classes with AM control algorithms
    class Flightmode{
        public:
        ControlTuning_t tuningData_;
        constexpr Flightmode(ControlTuning_t tuningData) : tuningData_(tuningData){}
        
        virtual void run() = 0;

        virtual void updatePid() = 0;
    };

}

#endif // ZPSW3_GLOBAL_CONFIG_HPP