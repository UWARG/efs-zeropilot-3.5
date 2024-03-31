#include "manual.hpp"
#include "CommonDataTypes.hpp"

namespace AM {

AttitudeManagerInput Manual::run(const AttitudeManagerInput& input, AxisPIDs _pids) {
    return input;
}

void Manual::updatePidGains(PIDController _axis, GainTerm whichGain, float desiredGain ) {
    switch (whichGain){
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

}  // namespace AM
