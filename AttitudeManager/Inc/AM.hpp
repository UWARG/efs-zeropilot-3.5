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

namespace AM {

class AttitudeManager {
   public:
    static void setControlInputs(const AttitudeManagerInput& new_control_inputs);

    static AttitudeManagerInput getControlInputs();

    AttitudeManager(Flightmode* control_algorithm);

    void runControlLoopIteration(const AttitudeManagerInput& instructions);

   private:
    AttitudeManager();
    void outputToMotor(uint8_t percent, uint8_t motorIndex);

    static SemaphoreHandle_t control_inputs_mutex;

    static struct AttitudeManagerInput control_inputs;

    Flightmode* control_algorithm_;
    MotorChannel *motorChannels_[config::NUM_MOTORS];
};


}  // namespace AM

#endif  // ZPSW3_AM_HPP
