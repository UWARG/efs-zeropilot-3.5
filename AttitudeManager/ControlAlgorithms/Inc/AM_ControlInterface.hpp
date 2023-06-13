/*
 * AM_ControlInterface.hpp
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

class ControlInterface {
   public:
    ControlInterface(uint8_t num_actuators) : num_actuators(num_actuators){};

    /// @brief Run the controls algorithm for the given flight model.
    ///
    /// @param instructions
    ///   The instructions for the flight model to follow
    virtual std::vector<ActuatorOutput> runControlsAlgorithm(
        const AttitudeManagerInput &instructions) = 0;

    virtual void updatePid() = 0;

    const uint8_t num_actuators;  // Number of actuators for given flight model

   private:
    ControlInterface();
};

typedef std::vector<ControlInterface *> ControlInterfaceList;

}  // namespace AM

#endif  // ZPSW3_AM_CONTROL_INTERFACE_HPP
