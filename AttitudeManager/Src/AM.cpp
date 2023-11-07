//
// Created by Anthony Luo on 2022-10-12.
//
#include "AM.hpp"

#include "FreeRTOS.h"
#include "semphr.h"

#include <array>
#include <cstdlib>

#include "main.h"

namespace AM {
SemaphoreHandle_t AttitudeManager::control_inputs_mutex = xSemaphoreCreateMutex();

AttitudeManagerInput AttitudeManager::control_inputs = {
    .roll = 1.0f,
    .pitch = 1.0f,
    .yaw = 1.0f,
    .throttle = 1.0f
};

void AttitudeManager::setControlInputs(const AttitudeManagerInput& new_control_inputs) {
    if (xSemaphoreTake(control_inputs_mutex, (TickType_t) portMAX_DELAY) == pdPASS) {
        control_inputs = new_control_inputs;
        xSemaphoreGive(control_inputs_mutex);
    }
}

AttitudeManagerInput AttitudeManager::getControlInputs() {
    AttitudeManagerInput temp {};
    if (xSemaphoreTake(control_inputs_mutex, (TickType_t) portMAX_DELAY) == pdPASS) {
        temp = control_inputs;
        xSemaphoreGive(control_inputs_mutex);
    }
    return temp;
}

void AttitudeManager::runControlLoopIteration(const AttitudeManagerInput& instructions) {
    myprintf("---------- IN RUN CONTROL LOOP ITERATION ----------\r\n");
    myprintf("R: %d, P: %d, Y: %d, T: %d\r\n\n", (int)instructions.roll, (int)instructions.pitch, (int)instructions.yaw, (int)instructions.throttle);
    // Process Instructions

    // Run Control Algorithms
    // control_algorithm->run();

    // Write motor outputs
}

}  // namespace AM
