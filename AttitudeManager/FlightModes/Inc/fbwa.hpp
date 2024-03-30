#ifndef ZPSW3_AM_FBWA_HPP
#define ZPSW3_AM_FBWA_HPP

#include "flightmode.hpp"

namespace AM {

class FBWA : public Flightmode {
   public:
    FBWA() = default;

    AttitudeManagerInput run(const AttitudeManagerInput& input, AxisPIDs _pids) override;
    void updatePid(PIDController _axis, GainTerm whichGain, float desiredGain) override;
    void updatePidGains() override;
    void updateControlLimits(ControlLimits_t limits) override;

   private:
    // TODO: FIXME to be a control limit class
    ControlLimits_t fbwa_control_limits;
};

}  // namespace AM

#endif  // ZPSW3_AM_FBWA_HPP
