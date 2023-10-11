//
// Created by Anthony Luo on 2022-10-12.
//
#include "AM.hpp"

#include "FreeRTOS.h"

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
    control_algorithm_(control_algorithm),
    motorInstances_(new MotorInstance_t[config::NUM_MOTORS]{})
{
    // Go through motor config list and count the # of motors of each axis type
    uint8_t yawCount{0}, pitchCount{0}, rollCount{0}, throttleCount{0};
    for (uint8_t i{0}; i < config::NUM_MOTORS; i++) {
        switch (config::motors[i].axis) {
            case config::yaw: 
                yawCount++;
                break;
            case config::pitch: 
                pitchCount++;
                break;
            case config::roll: 
                rollCount++;
                break;
        }
    }

    // Dedicate portions of the motorInstances array to the respective axis by holding references to those positions
    // We do this instead of creating 4 seperate arrays as we only know the # of total motors and thus don't know the size of the 4 arrays at compile time
    // If there is 0 of an axis, having the same reference as the next axis will cause it to do nothing
    motorReferences_[config::yaw] = &motorInstances_[0];
    motorReferences_[config::pitch] = &motorInstances_[yawCount];
    motorReferences_[config::roll] = &motorInstances_[yawCount + pitchCount];
    motorReferences_[config::throttle] = &motorInstances_[yawCount + pitchCount + rollCount];
    motorReferences_[4] = &motorInstances_[config::NUM_MOTORS]; // out of bounds reference, useful for outputToMotor method to know the end of the array

    // Initialize and store motor instances in the motorInstances array with respect to their axis
    yawCount = 0, pitchCount = 0, rollCount = 0, throttleCount = 0;
    for (uint8_t i{0}; i < config::NUM_MOTORS; i++) {
        switch (config::motors[i].axis) {
            case config::yaw:
                motorReferences_[config::yaw][yawCount].motorInstance = config::motors[i].driverConstructor();
                motorReferences_[config::yaw][yawCount].isInverted = config::motors[i].isInverted;
                yawCount++;
            case config::pitch:
                motorReferences_[config::pitch][pitchCount].motorInstance = config::motors[i].driverConstructor();
                motorReferences_[config::pitch][pitchCount].isInverted = config::motors[i].isInverted;
                pitchCount++;
            case config::roll:
                motorReferences_[config::roll][rollCount].motorInstance = config::motors[i].driverConstructor();
                motorReferences_[config::roll][rollCount].isInverted = config::motors[i].isInverted;
                rollCount++;
            case config::throttle:
                motorReferences_[config::throttle][throttleCount].motorInstance = config::motors[i].driverConstructor();
                motorReferences_[config::throttle][throttleCount].isInverted = config::motors[i].isInverted;
                throttleCount++;
        }
    }
};

void AttitudeManager::runControlLoopIteration(const AttitudeManagerInput& instructions) {
    // Process Instructions

    // Run Control Algorithms
    control_algorithm_->run();

    // Write motor outputs
}

void AttitudeManager::outputToMotor(config::ControlAxis_t axis, uint8_t percent) {
    // Move through the portion of the motorInstances array that matches the wanted axis.
    // The motorReferences array holds references to the wanted positions in the motorInstances array
    for (MotorInstance_t *i{motorReferences_[axis]}; i != motorReferences_[axis + 1]; i++) {
        if (i->isInverted) {
            i->motorInstance->set(100-percent);
        } else {
            i->motorInstance->set(percent);
        }
    }
    
}

}  // namespace AM
