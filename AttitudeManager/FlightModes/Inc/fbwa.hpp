#ifndef ZPSW3_AM_FBWA_HPP
#define ZPSW3_AM_FBWA_HPP

#include "flightmode.hpp"
#include "PID.hpp"

namespace AM {

class FBWA : public Flightmode {
   public:
    FBWA();

    AttitudeManagerInput run(const AttitudeManagerInput& input) override;
    void updatePid(ControlAxis_t axis, PIDController::PID newPIDVals) override;

   private:
    PIDController* pitchPID_;
    PIDController* rollPID_;
};

}  // namespace AM

#endif  // ZPSW3_AM_FBWA_HPP