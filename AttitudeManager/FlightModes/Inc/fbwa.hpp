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
};

}  // namespace AM

#endif  // ZPSW3_AM_FBWA_HPP
