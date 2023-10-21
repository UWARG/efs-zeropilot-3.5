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

AttitudeManager::AttitudeManager(Flightmode* controlAlgorithm, config::Motor_t motors[]):
    controlAlgorithm_(controlAlgorithm),
    motorInstances_(),
    numMotors()
{
    // Go through motor config list and count the # of motors of each axis type
    for (uint8_t i{0}; i < config::NUM_MOTORS; i++) {
        switch (motors[i].axis) {
            case config::yaw: 
                numMotors[config::yaw]++;
                break;
            case config::pitch: 
                numMotors[config::pitch]++;
                break;
            case config::roll: 
                numMotors[config::roll]++;
                break;
            case config::throttle:
                numMotors[config::throttle]++;
                break;
        }
    }

    motorInstances_[config::yaw] = new MotorInstance_t[numMotors[config::yaw]];
    motorInstances_[config::pitch] = new MotorInstance_t[numMotors[config::pitch]];
    motorInstances_[config::roll] = new MotorInstance_t[numMotors[config::roll]];
    motorInstances_[config::throttle] = new MotorInstance_t[numMotors[config::throttle]];
    
    // Initialize and store motor instances in the motorInstances array with respect to their axis
    uint8_t yawMotorCount{0}, pitchMotorCount{0}, rollMotorCount{0}, throttleMotorCount{0};
    for (uint8_t i{0}; i < config::NUM_MOTORS; i++) {
        switch (motors[i].axis) {
            case config::yaw:
                motorInstances_[config::yaw][yawMotorCount].motorInstance = motors[i].driverConstructor();
                motorInstances_[config::yaw][yawMotorCount].isInverted = motors[i].isInverted;
                yawMotorCount++;
                break;
            case config::pitch:
                motorInstances_[config::pitch][pitchMotorCount].motorInstance = motors[i].driverConstructor();
                motorInstances_[config::pitch][pitchMotorCount].isInverted = motors[i].isInverted;
                pitchMotorCount++;
                break;
            case config::roll:
                motorInstances_[config::roll][rollMotorCount].motorInstance = motors[i].driverConstructor();
                motorInstances_[config::roll][rollMotorCount].isInverted = motors[i].isInverted;
                rollMotorCount++;
                break;
            case config::throttle:
                motorInstances_[config::throttle][throttleMotorCount].motorInstance = motors[i].driverConstructor();
                motorInstances_[config::throttle][throttleMotorCount].isInverted = motors[i].isInverted;
                throttleMotorCount++;
                break;
        }
    }
};

AttitudeManager::~AttitudeManager() {
    for (uint8_t i{0}; i < 4; i++) {
        delete[] motorInstances_[i];
    }
}

void AttitudeManager::runControlLoopIteration(const AttitudeManagerInput& instructions) {
    // Process Instructions

    // Run Control Algorithms
    controlAlgorithm_->run();

    // Write motor outputs
}

void AttitudeManager::outputToMotor(config::ControlAxis_t axis, uint8_t percent) {
    // Move through the portion of the motorInstances array that matches the wanted axis.
    // The motorReferences array holds references to the wanted positions in the motorInstances array
    for (uint8_t motorCount{0}; motorCount < numMotors[axis]; motorCount++) {
        if (motorInstances_[axis][motorCount].isInverted) {
            motorInstances_[axis][motorCount].motorInstance->set(100-percent);
        } else {
            motorInstances_[axis][motorCount].motorInstance->set(percent);
        }
    }
    
}

}  // namespace AM
