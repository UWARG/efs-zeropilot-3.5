/*
 * AM_ControlAlgorithm.hpp
 *
 * Attitude Manager Control Algorithm Interface
 *
 * Created on: Oct 22, 2022
 * Author(s): Aidan Bowers
 */
#ifndef ZPSW3_AM_CONTROL_INTERFACE_HPP
#define ZPSW3_AM_CONTROL_INTERFACE_HPP

#include "AM_DataTypes.hpp"
#include "CommonDataTypes.hpp"

namespace AM {

class ControlAlgorithm {
   public:
    ControlAlgorithm(uint8_t num_actuators) : num_actuators(num_actuators){};

    /// @brief Run the controls algorithm for the given flight model.
    ///
    /// @param instructions
    ///   The instructions for the flight model to follow
    ///
    /// @returns Vector of outputs to be written to actuators
    virtual std::vector<ActuatorOutput> runControlsAlgorithm(
        const AttitudeManagerInput &instructions) = 0;

    virtual void updatePid() = 0;

    const uint8_t num_actuators;  // Number of actuators for given flight model

   private:
    ControlAlgorithm();
};

typedef std::vector<ControlAlgorithm *> ControlAlgorithmList;

}  // namespace AM

#endif  // ZPSW3_AM_CONTROL_INTERFACE_HPP
