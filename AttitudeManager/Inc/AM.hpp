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

#include "CommonDataTypes.hpp"
#include "FreeRTOS.h"
#include "flightmode.hpp"
#include "semphr.h"

namespace AM {

class AttitudeManager {
   public:
    AttitudeManager(Flightmode* control_algorithm) : control_algorithm(control_algorithm){};

    static void setControlInputs(const AttitudeManagerInput& new_control_inputs);
    static AttitudeManagerInput getControlInputs();

    void runControlLoopIteration();

   private:
    AttitudeManager() = delete;

    static SemaphoreHandle_t control_inputs_mutex;
    static struct AttitudeManagerInput control_inputs;

    Flightmode* control_algorithm;

    void outputToMotor(ControlAxis_t axis, uint8_t percent){};
};

}  // namespace AM

#endif  // ZPSW3_AM_HPP
