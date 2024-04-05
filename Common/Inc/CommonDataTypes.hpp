//
// Created by Gordon Fountain 2022-11-30.
//

#ifndef ZPSW3_COMMON_DATATYPES_HPP
#define ZPSW3_COMMON_DATATYPES_HPP

#include <stdint.h>

namespace AM {

struct AttitudeManagerInput {
    float roll;
    float pitch;
    float yaw;
    float throttle;
};

}  // Namespace AM

typedef enum {
    yaw,
    pitch,
    roll,
    throttle,
    NUM_CONTROL_AXIS
} ControlAxis_t;

using percentage_t = float;

typedef struct AxisLimits_t {
    percentage_t min;
    percentage_t max;
} AxisLimits_t;

typedef struct ControlLimits_t {
    AxisLimits_t yawLimit;
    AxisLimits_t pitchLimit;
    AxisLimits_t rollLimit;
    AxisLimits_t throttleLimit;
} ControlLimits_t;

#endif  // ZPSW3_COMMON_DATATYPES_HPP
