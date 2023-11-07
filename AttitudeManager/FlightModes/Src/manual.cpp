#include "manual.hpp"
#include "CommonDataTypes.hpp"

namespace AM {

AttitudeManagerInput Manual::run(const AttitudeManagerInput& input) {
    return input;
}

void Manual::updatePid(ControlAxis_t axis, PIDController::PID newPIDVals) {}

}  // namespace AM