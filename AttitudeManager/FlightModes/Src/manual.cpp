#include "manual.hpp"
#include "CommonDataTypes.hpp"

namespace AM {

AttitudeManagerInput Manual::run(const AttitudeManagerInput& input, AxisPIDs _pids) {
    return input;
}

void Manual::updatePid() {}
void Manual::updatePidGains(PIDController _axis, GainTerm whichGain, float desiredGain) {
    if (isnan(desiredGain)) {
        return;
    }

    switch (whichGain) {
        case proportional:
            _axis.setKp(desiredGain);
            break;
        case integral:
            _axis.setKi(desiredGain);
            break;
        case derivative:
            _axis.setKd(desiredGain);
            break;
    }
}
void Manual::updateControlLimits(ControlLimits_t limits) {}

}  // namespace AM
