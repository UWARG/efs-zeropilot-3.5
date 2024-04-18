/** CommonDataTypes.hpp
 * Contains a bunch of common datatypes!
 * 
 * Created by: Gordon Fountain 2022-11-30
 * Updated by: Anthony Luo
 * Last update: 2024-04-18
*/

#ifndef ZPSW3_COMMON_DATATYPES_HPP
#define ZPSW3_COMMON_DATATYPES_HPP

#include <stdint.h>

/************************************************
 * Sensor/Error structs (can be either or. tbd ig)
 ************************************************/
typedef enum SensorErrorCode {
    SENSOR_SUCCESS = 0,
    SENSOR_FAIL = -1,
    SENSOR_BUSY = 1,
} SensorErrorCode;

/************************************************
 * Control limits & inter-manager datatypes
 ************************************************/
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
