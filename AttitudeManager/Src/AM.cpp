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


SemaphoreHandle_t AttitudeManager::sensor_fusion_mutex = xSemaphoreCreateMutex();

SensorFusionOutput AttitudeManager::SF_data = {
    .roll = 0.0f,
    .pitch = 0.0f,
    .yaw = 0.0f,
    .throttle = 0.0f
};

void AttitudeManager::setSensorFusionData(const SensorFusionOutput& new_SF_output) {
    if (xSemaphoreTake(sensor_fusion_mutex, (TickType_t) portMAX_DELAY) == pdPASS) {
        SF_data = new_SF_output;
        xSemaphoreGive(sensor_fusion_mutex);
    }
}

SensorFusionOutput AttitudeManager::getSensorFusionData() {
    SensorFusionOutput latestData {};
    if (xSemaphoreTake(sensor_fusion_mutex, (TickType_t) portMAX_DELAY) == pdPASS) {
        latestData = SF_data;
        xSemaphoreGive(sensor_fusion_mutex);
    }
    return latestData;
}

//TODO: convert AM input format to degrees
float attitudePercentToDegrees(float input)
{
    return input;
}

//TODO: convert AM output format to percent
float attitudeDegreesToPercent(float output)
{
    return output;
}

void AttitudeManager::runControlLoopIteration() {
    // Process Instructions

    // Run Control Algorithms
    AttitudeManagerInput motorOutput = control_algorithm->run(getControlInputs());

    // Write motor outputs
}

}  // namespace AM
