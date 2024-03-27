#include "manual.hpp"
#include "CommonDataTypes.hpp"

namespace AM {

AttitudeManagerInput Manual::run(const AttitudeManagerInput& input) {
    return input;
}

void Manual::updatePid(PIDController _pid, PIDGains _gains) {
    _pid.updatePIDGains(_gains);
}

}  // namespace AM
