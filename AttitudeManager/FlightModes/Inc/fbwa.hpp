#ifndef ZPSW3_AM_FBWA_HPP
#define ZPSW3_AM_FBWA_HPP

#include "flightmode.hpp"

namespace AM {

class FBWA : public Flightmode {
   public:
    FBWA() = default;

    AttitudeManagerInput run(const AttitudeManagerInput& input) override;
    void updatePid() override;
    void updatePidGains() override;
    void updateControlLimits(ControlLimits_t limits) override;

    private:
    // TODO: FIXME to be a control limit class
    float yawMin;
    float yawMax;
    float rollMin;
    float rollMax;
    float pitchMin;
    float pitchMax;
    float throttleMin;
    float throttleMax;
};

}  // namespace AM

#endif  // ZPSW3_AM_FBWA_HPP
