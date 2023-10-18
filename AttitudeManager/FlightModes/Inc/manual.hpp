#ifndef ZPSW3_AM_MANUAL_HPP
#define ZPSW3_AM_MANUAL_HPP

#include "flightmode.hpp"

namespace AM {

class Manual : public Flightmode {
   public:
    Manual() = default;

    AttitudeManagerInput run(const AttitudeManagerInput& input) override;
    void updatePid(ControlAxis_t axis, PIDController::PID newPIDVals) override;
};

}  // namespace AM

#endif  // ZPSW3_AM_MANUAL_HPP