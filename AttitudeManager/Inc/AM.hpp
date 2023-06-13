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

#include "AM_ControlInterface.hpp"
#include "AM_DataTypes.hpp"
#include "CommonDataTypes.hpp"
#include "cmsis_os.h"

namespace AM {

class AttitudeManager {
   public:
    AttitudeManager(ControlInterfaceList controller_interfaces)
        : controller_interfaces(controller_interfaces){};

    // https://en.cppreference.com/w/cpp/language/parameter_pack
    template <typename... Args>
    AttitudeManager(Args... controllers) : controller_interfaces{controllers...} {};

    void runControlLoopIteration(const AttitudeManagerInput &instructions);

    void setSmQueue(osMessageQId queueId);
    osMessageQId getSmQueue();

   private:
    AttitudeManager();

    uint8_t current_controller_index = 0;
    uint8_t desired_controller_index = 0;
    const ControlInterfaceList controller_interfaces;
    LOS::LosSFData current;
    float desired_airspeed = 0;  // could this be determined by our desired controller index?
    float current_airspeed = 0;
    float transition_start_airspeed = 0;

    // Only stored in AM, reading and clearing done in SM code.
    osMessageQId SM_to_AM_queue;

    void setDesiredControlAlgorithm(uint8_t id);

    std::vector<ActuatorOutput> runTransitionMixingIteration(
        ControlInterface *current, ControlInterface *desired,
        const AM::AttitudeManagerInput &instructions);
};
}  // namespace AM

#endif  // ZPSW3_AM_HPP
