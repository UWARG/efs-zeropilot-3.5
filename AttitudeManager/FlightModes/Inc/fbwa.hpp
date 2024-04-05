#ifndef ZPSW3_AM_FBWA_HPP
#define ZPSW3_AM_FBWA_HPP

#include "flightmode.hpp"

namespace AM {

class FBWA : public Flightmode {
   public:
    FBWA() = default;

    AttitudeManagerInput run(const AttitudeManagerInput& input) override;
    void updatePid() override;
    void updatePidGains(PIDController _axis, GainTerm whichGain, float desiredGain) override;
    void updateControlLimits() override;

   private:
    // TODO: FIXME to be a control limit class
    ControlLimits_t fbwa_control_limits;
};

}  // namespace AM

#endif  // ZPSW3_AM_FBWA_HPP
