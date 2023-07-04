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

    //TODO: Determine correct location for motor driver definitions, and finish implementation.
    class MotorDriver{};
    class TempPWMDriver : public MotorDriver{};
    class TempDSHOTDriver : public MotorDriver{};

    //Define factory function to create motor drivers.
    //The driver class must be copy-constructable as the returned object will be a copy.
    template <class DriverType>
    MotorDriver constructDriver() {
        DriverType driver;
        return driver;
    }

    //Typedef pointer to factory function
    typedef MotorDriver (*DriverFactory)(void);

    typedef struct {
        ControlAxis_t axis;
        bool isInverted;
        DriverFactory driverConstructor;
        //TODO: Consider a more specific datatype for interfaceID
        //(e.g. a datatype defined within the motor driver class)
        uint8_t interfaceID;
    } Motor_t;

    typedef struct {
        bool isEnabled;
        float p;
        float i;
        float d;
    } AxisPID_t;

    typedef struct {
        AxisPID_t yawPID;
        AxisPID_t pitchPID;
        AxisPID_t rollPID;
        AxisPID_t thrustPID;
    } ControlPID_t;

    typedef struct {
        float min;
        float max;
    } AxisLimits_t;

    typedef struct {
        AxisLimits_t yawLimit;
        AxisLimits_t pitchLimit;
        AxisLimits_t rollLimit;
        AxisLimits_t thrustLimit;
    } ControlLimits_t;

    //TODO: Combine flightmode classes with AM control algorithms
    class Flightmode{
        public:
        ControlPID_t PIDValues_;
        ControlLimits_t controlLimits_;
        constexpr Flightmode(ControlPID_t pid, ControlLimits_t limits): PIDValues_(pid), controlLimits_(limits){}
    };

}

#endif // ZPSW3_GLOBAL_CONFIG_HPP