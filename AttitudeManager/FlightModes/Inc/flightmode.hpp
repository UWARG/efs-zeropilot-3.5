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
    virtual ~Flightmode() = default;

    virtual AttitudeManagerInput run(const AttitudeManagerInput& input) = 0;
    virtual void updatePid() = 0;
    virtual void updatePidGains();
    virtual void updateControlLimits(ControlLimits_t limits);

   protected:
    Flightmode() = default;
};

}  // namespace AM

#endif  // ZPSW3_AM_CONTROL_INTERFACE_HPP
