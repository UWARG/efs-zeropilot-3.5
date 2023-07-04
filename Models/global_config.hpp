#include <cstdint>

typedef enum {
    yaw,
    pitch,
    roll,
    thrust
} ControlAxis_t;

typedef enum {
    positive,
    negative
} ControlDirection_t;

//TODO: Determine correct location for motor driver definitions, and finish implementation.
class MotorDriver{};
class TempPWMDriver : public MotorDriver{};
class TempDSHOTDriver : public MotorDriver{};

typedef struct {
    ControlAxis_t axis;
    ControlDirection_t direction;
    MotorDriver driver;
    //TODO: Consider a more specific datatype for interfaceID
    //(e.g. a datatype defined within the motor driver class)
    uint8_t interfaceID;
} Motor_t;

typedef struct {
    float p;
    float i;
    float d;
} PID_t;

typedef struct {
    float min;
    float max;
} AxisLimit_t;

typedef struct {
    AxisLimit_t yawLimit;
    AxisLimit_t pitchLimit;
    AxisLimit_t rollLimit;
    AxisLimit_t thrustLimit;
} ControlLimits_t;

//TODO: Combine flightmode classes with AM control algorithms
class Flightmode{
    PID_t PIDValues;
    ControlLimits_t controlLimits;
};