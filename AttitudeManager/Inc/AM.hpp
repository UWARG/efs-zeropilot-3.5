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
#include "AM_DataTypes.hpp"
#include "CommonDataTypes.hpp"

namespace AM {

class AttitudeManager {
   public:
    AttitudeManager(ControlAlgorithmList control_algorithms)
        : control_algorithms(control_algorithms){};

    // https://en.cppreference.com/w/cpp/language/parameter_pack
    template <typename... Args>
    AttitudeManager(Args... controllers) : control_algorithms{controllers...} {}

    void runControlLoopIteration(const AttitudeManagerInput &instructions);

   private:
    AttitudeManager();

    uint8_t current_controller_index = 0;
    uint8_t desired_controller_index = 0;
    const ControlAlgorithmList control_algorithms;
    LOS::LosSFData current;
    float desired_airspeed = 0;  // could this be determined by our desired controller index?
    float current_airspeed = 0;
    float transition_start_airspeed = 0;

    void setDesiredControlAlgorithm(uint8_t id);

    std::vector<ActuatorOutput> runTransitionMixingIteration(
        ControlAlgorithm *current, ControlAlgorithm *desired,
        const AM::AttitudeManagerInput &instructions);
};
}  // namespace AM

#endif  // ZPSW3_AM_HPP
