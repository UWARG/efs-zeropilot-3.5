#ifndef ZPSW3_AM_MANUAL_HPP
#define ZPSW3_AM_MANUAL_HPP

#include "flightmode.hpp"

namespace AM {

class Manual : public Flightmode {
   public:
    Manual() = default;

    AttitudeManagerInput run(const AttitudeManagerInput& input) override;
    void updatePid() override;
    void updatePidGains(PidAxis pid_axis, GainTerm pid_gain_term, float desired_gain) override;
    void updateControlLimits(ControlLimits_t limits) override;

   private:
    PIDController pitchPID;
    PIDController rollPID;
    PIDController yawPID;
    PIDController throttlePID;
};

}  // namespace AM

#endif  // ZPSW3_AM_MANUAL_HPP
