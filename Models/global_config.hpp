#ifndef ZPSW3_GLOBAL_CONFIG_HPP
#define ZPSW3_GLOBAL_CONFIG_HPP

#include <cstdint>

namespace config
{

    typedef enum {
        yaw,
        pitch,
        roll,
        thrust
    } ControlAxis_t;

    //TODO: Include drivers to replace all of these temporary definitions
    class MotorDriver
    {
        public:
        struct MotorIID{};
    };
        class TempPWMDriver : public MotorDriver
        {
            //TODO: implement an interface identifier (IID) for each driver class
            public:
            struct PWMIID : public MotorDriver::MotorIID{
                uint8_t timerID;
                uint16_t timerChannel;
                //constexpr constructor allows for compile-time configuration
                constexpr PWMIID(const uint8_t timerID, const uint16_t timerChannel)
                : timerID(timerID), timerChannel(timerChannel){}
            };
        };
        class TempDSHOTDriver : public MotorDriver
        {
            public:
            struct DSHOTIID : public MotorDriver::MotorIID{
                constexpr DSHOTIID(){}
            };
        };
    class RCInputDriver
    {
        public:
        struct RCInputIID{};
    };
        class TempPPMDriver : public RCInputDriver
        {
            public:
            struct PPMIID : public RCInputDriver::RCInputIID{
                constexpr PPMIID(){};
            };
        };
        class TempSBusDriver : public RCInputDriver
        {
            public:
            struct SBusIID : public RCInputDriver::RCInputIID{
                constexpr SBusIID(){};
            };
        };
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
    template <class BaseDriverType, class DerivedDriverType>
    BaseDriverType constructDriver() {
        DerivedDriverType driver;
        return driver;
    }

    //Typedef pointer to factory function
    template <class BaseDriverType>
    using DriverFactory = BaseDriverType (*)(void);

    typedef struct {
        ControlAxis_t axis;
        bool isInverted = false;
        DriverFactory<MotorDriver> driverConstructor;
        MotorDriver::MotorIID interfaceID;
    } Motor_t;

    typedef struct {
        //TODO: determine other config fields relevant to the RC input type
        DriverFactory<RCInputDriver> driverConstructor;
        RCInputDriver::RCInputIID interfaceID;
    } RCInput_t;

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