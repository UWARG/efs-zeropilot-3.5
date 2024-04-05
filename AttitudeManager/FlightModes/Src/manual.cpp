#include "manual.hpp"
#include "CommonDataTypes.hpp"

namespace AM {

AttitudeManagerInput Manual::run(const AttitudeManagerInput& input, AxisPIDs _pids) {
    return input;
}

void Manual::updatePid() {}
void Manual::updatePidGains() {}
void Manual::updateControlLimits(ControlLimits_t limits) {}

}  // namespace AM
