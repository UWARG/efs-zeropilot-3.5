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
    uint8_t numYawMotors{0}, numPitchMotors{0}, numRollMotors{0}, numThrottleMotors{0};
    for (uint8_t i{0}; i < config::NUM_MOTORS; i++) {
        switch (config::motors[i].axis) {
            case config::yaw: 
                numYawMotors++;
                break;
            case config::pitch: 
                numPitchMotors++;
                break;
            case config::roll: 
                numRollMotors++;
                break;
        }
    }

    // Dedicate portions of the motorInstances array to the respective axis by holding references to those positions
    // We do this instead of creating 4 seperate arrays as we only know the # of total motors and thus don't know the size of the 4 arrays at compile time
    // If there is 0 of an axis, having the same reference as the next axis will cause it to do nothing
    motorReferences_[config::yaw] = &motorInstances_[0];
    motorReferences_[config::pitch] = &motorInstances_[numYawMotors];
    motorReferences_[config::roll] = &motorInstances_[numYawMotors + numPitchMotors];
    motorReferences_[config::throttle] = &motorInstances_[numYawMotors + numPitchMotors + numRollMotors];
    motorReferences_[4] = &motorInstances_[config::NUM_MOTORS]; // out of bounds reference, useful for outputToMotor method to know the end of the array

    // Initialize and store motor instances in the motorInstances array with respect to their axis
    numYawMotors = 0, numPitchMotors = 0, numRollMotors = 0, numThrottleMotors = 0;
    for (uint8_t i{0}; i < config::NUM_MOTORS; i++) {
        switch (config::motors[i].axis) {
            case config::yaw:
                motorReferences_[config::yaw][numYawMotors].motorInstance = config::motors[i].driverConstructor();
                motorReferences_[config::yaw][numYawMotors].isInverted = config::motors[i].isInverted;
                numYawMotors++;
                break;
            case config::pitch:
                motorReferences_[config::pitch][numPitchMotors].motorInstance = config::motors[i].driverConstructor();
                motorReferences_[config::pitch][numPitchMotors].isInverted = config::motors[i].isInverted;
                numPitchMotors++;
                break;
            case config::roll:
                motorReferences_[config::roll][numRollMotors].motorInstance = config::motors[i].driverConstructor();
                motorReferences_[config::roll][numRollMotors].isInverted = config::motors[i].isInverted;
                numRollMotors++;
                break;
            case config::throttle:
                motorReferences_[config::throttle][numThrottleMotors].motorInstance = config::motors[i].driverConstructor();
                motorReferences_[config::throttle][numThrottleMotors].isInverted = config::motors[i].isInverted;
                numThrottleMotors++;
                break;
        }
    }
};

AttitudeManager::~AttitudeManager() {
    delete[] motorInstances_;
    motorInstances_ = nullptr;
    
    for (int i = 0; i < 5; i++)
    {
        motorReferences_[i] = nullptr;
    }
}

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
