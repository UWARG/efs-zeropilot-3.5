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

#include <vector>

namespace AM {

class AttitudeManager {
   public:
    static void setControlInputs(const AttitudeManagerInput& new_control_inputs);

    static AttitudeManagerInput getControlInputs();

    AttitudeManager(Flightmode* control_algorithm);

    void runControlLoopIteration(const AttitudeManagerInput& instructions);

   private:
    AttitudeManager();
    void outputToMotor(config::ControlAxis_t axis, uint8_t percent);

    static SemaphoreHandle_t control_inputs_mutex;

    static struct AttitudeManagerInput control_inputs;

    Flightmode* control_algorithm_;
    MotorInstance_t motorInstances_[config::NUM_MOTORS]{};
    MotorInstance_t *motorReferences_[5]{nullptr};
    
};

// !!unsure if the struct definition should be here, would appreciate feedback
typedef struct {
        MotorChannel *motorInstance;
        bool isInverted;
    } MotorInstance_t;

}  // namespace AM

#endif  // ZPSW3_AM_HPP
