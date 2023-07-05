//
// Created by Anthony Luo on 2022-10-12.
//
#include "AM.hpp"

#include <array>
#include <cstdlib>

namespace AM {

AttitudeManager::AttitudeManager(ControlAlgorithm* control_algorithm, MotorType_e motorType):
    control_algorithm(control_algorithm),
    motorChannel_(nullptr)
{
    switch (motorType) {

        case (PWM):
            // motorChannel_ = new PWMChannel(timer, timer_channel);
            break;

        case (DSHOT):
            // motorChannel_ = new DSHOTChannel(pin_num, port, timer, timer_channel, tim_DMA_handle_index, tim_dma_source);
            break;

        default:
            motorChannel_ = nullptr; // Should I log some sort of error msg here?


    }
};

void AttitudeManager::runControlLoopIteration(const AttitudeManagerInput &instructions) {
    // Process Instructions

    // Run Control Algorithms
    control_algorithm->run();

    // Write motor outputs
}

void AttitudeManager::outputToMotor(uint8_t percent) {
    motorChannel_->set(percent);
}

}  // namespace AM
