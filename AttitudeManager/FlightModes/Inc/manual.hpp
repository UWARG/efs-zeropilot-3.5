#ifndef ZPSW3_AM_MANUAL_HPP
#define ZPSW3_AM_MANUAL_HPP

#include "flightmode.hpp"
#include "PID.hpp"

namespace AM {

class Manual : public Flightmode {
    public:
        Manual() = default;

        AttitudeManagerInput run(const AttitudeManagerInput& input) override;
        void updatePid(AxisPIDs _pids, ControlAxis_t axis, PIDGains newPIDVals) override;
};

}  // namespace AM

#endif  // ZPSW3_AM_MANUAL_HPP