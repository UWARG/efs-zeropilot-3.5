#ifndef ZPSW3_AM_FBWA_HPP
#define ZPSW3_AM_FBWA_HPP

#include "flightmode.hpp"
#include "PID.hpp"
#include "config_foundation.hpp"

namespace AM {

class FBWA : public Flightmode {
   public:
    // FBWA() = default;
    FBWA(const config::Flightmode_t& config) : tuningData(config.tuningData) {};

    AttitudeManagerInput run(const AttitudeManagerInput& input) override;
    void updatePid() override;
    void updatePidGains() override;
    void updateControlLimits(ControlLimits_t limits) override;

   private:
    // TODO: FIXME to be a control limit class
    ControlLimits_t fbwa_control_limits;
    config::ControlTuning_t tuningData;

    // PIDController::PID pitch_pids = {0.1, 0.1, 0.1, 1.0, -100.0, +100.0};
    PIDController pitch_pid_controller{tuningData.PIDValues.pitchPID};
    PIDController roll_pid_controller{tuningData.PIDValues.rollPID};
    PIDController yaw_pid_controller{tuningData.PIDValues.yawPID};
};

}  // namespace AM

#endif  // ZPSW3_AM_FBWA_HPP
