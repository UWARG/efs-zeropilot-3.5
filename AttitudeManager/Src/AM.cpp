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

AxisPIDs axisPIDs = {
    .pitch = PIDController(flightmodes[1].tuningData.PIDValues.pitchPID.p,
                        flightmodes[1].tuningData.PIDValues.pitchPID.i,
                        flightmodes[1].tuningData.PIDValues.pitchPID.d, 100, 0, 100, 0, 0, 0),
    .roll = PIDController(flightmodes[1].tuningData.PIDValues.rollPID.p, 
                        flightmodes[1].tuningData.PIDValues.rollPID.i,
                        flightmodes[1].tuningData.PIDValues.rollPID.d, 100, 0, 100, 0, 0, 0),
    .yaw = PIDController(flightmodes[1].tuningData.PIDValues.yawPID.p, 
                        flightmodes[1].tuningData.PIDValues.yawPID.i, 
                        flightmodes[1].tuningData.PIDValues.yawPID.d, 100, 0, 100, 0, 0, 0),
    .throttle = PIDController(0, 0, 0, 0, 0, 0, 0, 0, 0)
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

void AttitudeManager::runControlLoopIteration() {
    // Process Instructions
    AttitudeManagerInput control_inputs = getControlInputs();

    // Run Control Algorithms
    AttitudeManagerInput motor_outputs = controlAlgorithm_->run(control_inputs, axisPIDs);

    // Write motor outputs
    outputToMotor(yaw, static_cast<uint8_t>(motor_outputs.yaw));
    outputToMotor(pitch, static_cast<uint8_t>(motor_outputs.pitch));
    outputToMotor(roll, static_cast<uint8_t>(motor_outputs.roll));
    outputToMotor(throttle, static_cast<uint8_t>(motor_outputs.throttle));

}

void AttitudeManager::outputToMotor(ControlAxis_t axis, uint8_t percent) {
    // Move through the portion of the motorInstances array that matches the wanted axis.
    // The motorReferences array holds references to the wanted positions in the motorInstances array
    for (uint8_t motorCount{0}; motorCount < numMotorsPerAxis_[axis]; ++motorCount) {
        if (motorInstances_[axis][motorCount].isInverted) {
            motorInstances_[axis][motorCount].motorInstance->set(100-percent);
        } else {
            motorInstances_[axis][motorCount].motorInstance->set(percent);
        }
    }

}

}  // namespace AM
