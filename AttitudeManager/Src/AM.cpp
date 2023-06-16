//
// Created by Anthony Luo on 2022-10-12.
//
#include "AM.hpp"

#include <array>
#include <cstdlib>

namespace AM {

void AttitudeManager::runControlLoopIteration(const AttitudeManagerInput &instructions) {
    // Process Instructions

    // Run Control Algorithms
    control_algorithm->run();

    // Write motor outputs
}

}  // namespace AM
