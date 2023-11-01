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

AttitudeManager::AttitudeManager(Flightmode* controlAlgorithm,  MotorInstance_t *(&motorInstances)[], uint8_t (&numMotorsPerAxis)[]):
    controlAlgorithm_(controlAlgorithm),
    motorInstances_(motorInstances),
    numMotorsPerAxis_(numMotorsPerAxis)
{};

AttitudeManager::~AttitudeManager() 
{}

void AttitudeManager::runControlLoopIteration(const AttitudeManagerInput& instructions) {
    // Process Instructions

    // Run Control Algorithms
    controlAlgorithm_->run();

    // Write motor outputs
}

void AttitudeManager::outputToMotor(config::ControlAxis_t axis, uint8_t percent) {
    // Move through the portion of the motorInstances array that matches the wanted axis.
    // The motorReferences array holds references to the wanted positions in the motorInstances array
    for (uint8_t motorCount{0}; motorCount < numMotorsPerAxis_[axis]; motorCount++) {
        if (motorInstances_[axis][motorCount].isInverted) {
            motorInstances_[axis][motorCount].motorInstance->set(100-percent);
        } else {
            motorInstances_[axis][motorCount].motorInstance->set(percent);
        }
    }
    
}

}  // namespace AM
