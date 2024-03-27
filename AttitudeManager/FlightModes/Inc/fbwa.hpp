#ifndef ZPSW3_AM_FBWA_HPP
#define ZPSW3_AM_FBWA_HPP

#include "flightmode.hpp"

namespace AM {

class FBWA : public Flightmode {
   public:
    FBWA();

    AttitudeManagerInput run(const AttitudeManagerInput& input) override;
    void updatePid(AxisPIDs _pids, ControlAxis_t axis, PIDGains newPIDVals) override;

   private:
    PIDController* pitchPID_;
    PIDController* rollPID_;
};

}  // namespace AM

#endif  // ZPSW3_AM_FBWA_HPP