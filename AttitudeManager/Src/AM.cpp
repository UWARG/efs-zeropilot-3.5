//
// Created by Anthony Luo on 2022-10-12.
//
#include "AM.hpp"

#include <array>
#include <cstdlib>

#include "LOS_Actuators.hpp"

namespace AM {

void AttitudeManager::runControlLoopIteration(const AttitudeManagerInput &instructions) {
    // Process Instructions

    // Run Control Algorithms
    std::vector<ActuatorOutput> controller_output;
    if (current_controller_index == desired_controller_index) {
        controller_output =
            controller_interfaces[current_controller_index]->runControlsAlgorithm(instructions);
    } else {
        // Handle transition between flight modes
        controller_output = runTransitionMixingIteration(
            controller_interfaces[current_controller_index],
            controller_interfaces[desired_controller_index], instructions);
    }

    // Write actuator outputs
    for (auto output : controller_output) {
        Los_Actuators::getInstance().set(output.channel, output.percent);
    }
}

void AttitudeManager::setDesiredControlAlgorithm(uint8_t id) {
    // Only change desired controller if we aren't trying to transition already
    if (id < controller_interfaces.size() && desired_controller_index == current_controller_index) {
        desired_controller_index = id;

        transition_start_airspeed = abs(current.airspeed);
    }
}

std::vector<ActuatorOutput> AttitudeManager::runTransitionMixingIteration(
    ControlInterface *current_controller, ControlInterface *desired_controller,
    const AttitudeManagerInput &instructions) {
    // Run control algorithms to mix
    std::vector<ActuatorOutput> current_output =
        current_controller->runControlsAlgorithm(instructions);
    std::vector<ActuatorOutput> desired_output =
        desired_controller->runControlsAlgorithm(instructions);

    // Determine how much of each mix to apply
    current_airspeed = abs(current.airspeed);
    const float transition_percent = (transition_start_airspeed - current_airspeed) /
                                     (transition_start_airspeed - desired_airspeed);
    const float inv_transition_percent = 1 - transition_percent;

    if (((desired_airspeed - 2) <= current_airspeed &&
         current_airspeed <= (desired_airspeed + 2))) {
        // Update the active controller
        current_controller_index = desired_controller_index;
    }

    // Mix the outputs
    std::vector<ActuatorOutput> mixed_output = {};
    for (auto current_actuator : current_output) {
        ActuatorOutput value = {current_actuator.channel,
                                current_actuator.percent * inv_transition_percent};

        for (auto desired_actuator : desired_output) {
            if (current_actuator.channel == desired_actuator.channel) {
                value.percent += desired_actuator.percent * transition_percent;
            }
        }
        mixed_output.push_back(value);
    }

    return mixed_output;
}

}  // namespace AM
