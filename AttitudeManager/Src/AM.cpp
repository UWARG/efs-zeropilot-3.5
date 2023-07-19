//
// Created by Anthony Luo on 2022-10-12.
//
#include "AM.hpp"

#include "FreeRTOS.h"
#include "semphr.h"
#include "config_foundation.hpp"
#include "config.hpp"

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

AttitudeManager::AttitudeManager(Flightmode* control_algorithm):
    control_algorithm_(control_algorithm)
{
    for (uint8_t i{0}; i < config::NUM_MOTORS; i++) {
        motorChannel_[i] = config::motors[i].driverConstructor();   
    }
};

void AttitudeManager::runControlLoopIteration(const AttitudeManagerInput& instructions) {
    // Process Instructions

    // Run Control Algorithms
    control_algorithm_->run();

    // Write motor outputs
}

void AttitudeManager::outputToMotor(uint8_t percent, uint8_t motorIndex) {
    motorChannel_[motorIndex]->set(percent);
}

}  // namespace AM
