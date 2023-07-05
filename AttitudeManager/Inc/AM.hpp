/*
 * AM.hpp
 *
 * Attitude Manager Header
 *
 * Created on: Oct 12, 2022
 * Author(s): Anthony (anni) Luo; Dhruv Upadhyay
 */
#ifndef ZPSW3_AM_HPP
#define ZPSW3_AM_HPP

#include "AM_ControlAlgorithm.hpp"
#include "CommonDataTypes.hpp"
#include "ZP_D_PWMChannel.hpp"

namespace AM {

class AttitudeManager {
   public:
    AttitudeManager(ControlAlgorithm* control_algorithm);

    void runControlLoopIteration(const AttitudeManagerInput& instructions);

   private:
    AttitudeManager();
    void outputToMotor(uint8_t percent);

    ControlAlgorithm* control_algorithm;
    MotorChannel* motorChannel_;
};
}  // namespace AM

#endif  // ZPSW3_AM_HPP
