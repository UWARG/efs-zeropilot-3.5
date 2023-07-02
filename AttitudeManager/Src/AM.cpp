//
// Created by Anthony Luo on 2022-10-12.
//
#include "AM.hpp"
#include "semphr.h"

#include <array>
#include <cstdlib>

namespace AM {
AttitudeManager::control_inputs_mutex = xSemaphoreCreateMutex();

AttitudeManager::control_inputs = {
    .roll = 0.0f,
    .pitch = 0.0f,
    .yaw = 0.0f,
    .throttle = 0.0f
};

void AttitudeManager::setControlInputs(const AttitudeManagerInput& new_control_inputs) {
    xSemaphoreTake( control_inputs_mutex );
    control_inputs = new_control_inputs;
    xSemaphoreGive( control_inputs_mutex );
}

AttitudeManagerInput getControlInputs() {
    struct AttitudeManagerInput temp;
    xSemaphoreTake( control_inputs_mutex );
    temp = control_inputs;
    xSemaphoreGive( control_inputs_mutex );
    return temp;
}

void AttitudeManager::runControlLoopIteration(const AttitudeManagerInput& instructions) {
    // Process Instructions

    // Run Control Algorithms
    control_algorithm->run();

    // Write motor outputs
}

}  // namespace AM
