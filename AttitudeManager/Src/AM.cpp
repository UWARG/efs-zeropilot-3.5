//
// Created by Anthony Luo on 2022-10-12.
//
#include "AM.hpp"

#include "FreeRTOS.h"
#include "semphr.h"

#include <array>
#include <cstdlib>

namespace AM {
SemaphoreHandle_t AttitudeManager::control_inputs_mutex = xSemaphoreCreateMutex();

AttitudeManagerInput AttitudeManager::control_inputs = {
    .roll = 0.0f,
    .pitch = 0.0f,
    .yaw = 0.0f,
    .throttle = 0.0f
};

void AttitudeManager::setControlInputs(const AttitudeManagerInput& new_control_inputs) {
    xSemaphoreTake( control_inputs_mutex, (TickType_t) 10);
    control_inputs = new_control_inputs;
    xSemaphoreGive( control_inputs_mutex );
}

AttitudeManagerInput AttitudeManager::getControlInputs() {
    struct AttitudeManagerInput temp;
    xSemaphoreTake( control_inputs_mutex, (TickType_t) 10);
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
