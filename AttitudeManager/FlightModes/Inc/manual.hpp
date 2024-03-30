#ifndef ZPSW3_AM_MANUAL_HPP
#define ZPSW3_AM_MANUAL_HPP

#include "flightmode.hpp"

namespace AM {

class Manual : public Flightmode {
   public:
    Manual() = default;

    AttitudeManagerInput run(const AttitudeManagerInput& input, AxisPIDs _pids) override;
    void updatePid(PIDController _axis, GainTerm whichGain, float desiredGain ) override;
    void updatePidGains() override;
    void updateControlLimits(ControlLimits_t limits) override;
};

}  // namespace AM

#endif  // ZPSW3_AM_MANUAL_HPP
