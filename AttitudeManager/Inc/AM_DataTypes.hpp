//
// Created by Anthony Luo on 2022-10-13.
//

#ifndef ZPSW3_AM_DATATYPES_HPP
#define ZPSW3_AM_DATATYPES_HPP

#include <stdint.h>

#include <vector>

#include "constrain.h"

namespace AM {

class StateMix {
   public:
    const float velocity_x, velocity_y, velocity_z, pitch, roll, yaw;
    StateMix(float velocity_x, float velocity_y, float velocity_z, float pitch, float roll,
             float yaw)
        : velocity_x(constrain<float, 1, -1>(velocity_x)),
          velocity_y(constrain<float, 1, -1>(velocity_y)),
          velocity_z(constrain<float, 1, -1>(velocity_z)),
          pitch(constrain<float, 1, -1>(pitch)),
          roll(constrain<float, 1, -1>(roll)),
          yaw(constrain<float, 1, -1>(yaw)) {}
};

struct ActuatorOutput {
    uint8_t channel;
    float percent;
};

}  // namespace AM

#endif  // ZPSW3_AM_DATATYPES_HPP
