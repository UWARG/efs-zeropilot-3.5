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
#include "semphr.h"
#include "config_foundation.hpp"
#include "config.hpp"

namespace AM {

// Test class forward declarations
class MotorInitializationAndOutput;
// End of test class forward declarations

class AttitudeManager {
   public:
    static void setControlInputs(const AttitudeManagerInput& new_control_inputs);

    static AttitudeManagerInput getControlInputs();

    AttitudeManager(Flightmode* control_algorithm);

    ~AttitudeManager();

    void runControlLoopIteration(const AttitudeManagerInput& instructions);

   private:
    FRIEND_TEST(AttitudeManager, MotorInitializationAndOutput);

    AttitudeManager();
    void outputToMotor(config::ControlAxis_t axis, uint8_t percent);

    typedef struct {
        MotorChannel *motorInstance;
        bool isInverted;
    } MotorInstance_t;

    static SemaphoreHandle_t control_inputs_mutex;

    static struct AttitudeManagerInput control_inputs;

    Flightmode *control_algorithm_;
    MotorInstance_t *motorInstances_;
    MotorInstance_t *motorReferences_[5]{nullptr};
    
};

}  // namespace AM

#endif  // ZPSW3_AM_HPP
