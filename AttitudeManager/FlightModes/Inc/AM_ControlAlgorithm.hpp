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

#include "CommonDataTypes.hpp"

namespace AM {

class Flightmode {
   public:
    Flightmode(){};

    /// @brief Run the controls algorithm for the given flight model.
    /// @param
    /// @returns
    virtual void run() = 0;

    virtual void updatePid() = 0;
};

}  // namespace AM

#endif  // ZPSW3_AM_CONTROL_INTERFACE_HPP
