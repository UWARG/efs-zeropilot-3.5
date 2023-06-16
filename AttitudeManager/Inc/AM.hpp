/*
 * AM.hpp
 *
 * Attitude Manager Header
 *
 * Created on: Oct 12, 2022
 * Author(s): Anthony (anni) Luo; Dhruv Upadhyay
 */
#ifndef ZPSW3_AM_HPP
#define ZPSW3_AM_HPP

#include "AM_ControlAlgorithm.hpp"
#include "CommonDataTypes.hpp"

namespace AM {

class AttitudeManager {
   public:
    AttitudeManager(ControlAlgorithm* control_algorithm) : control_algorithm(control_algorithm){};

    void runControlLoopIteration(const AttitudeManagerInput& instructions);

   private:
    AttitudeManager();

    ControlAlgorithm* control_algorithm;
};
}  // namespace AM

#endif  // ZPSW3_AM_HPP
