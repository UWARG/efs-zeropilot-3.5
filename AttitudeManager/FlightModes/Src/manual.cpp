#include "manual.hpp"
#include "PID.hpp"
#include "CommonDataTypes.hpp"

namespace AM {

AttitudeManagerInput Manual::run(const AttitudeManagerInput& input) {
    return input;
}

void Manual::updatePid(AxisPIDs _pids, ControlAxis_t axis, PIDGains newPIDVals) {
    switch (axis) {
        case yaw:
            _pids.yawPID.updatePIDGains(newPIDVals);
            break;
        case pitch:
            _pids.pitchPID.updatePIDGains(newPIDVals);
            break;
        case roll:
            _pids.rollPID.updatePIDGains(newPIDVals);
            break;
        case thrust:
            _pids.thrustPID.updatePIDGains(newPIDVals);
            break;
    }
}

}  // namespace AM