#include "manual.hpp"
#include "CommonDataTypes.hpp"

namespace AM {

AttitudeManagerInput Manual::run(const AttitudeManagerInput& input) {
    return input;
}

void Manual::updatePid(AxisPIDs _pids, ControlAxis_t controlAxis, GainTerm _whichGain, float desiredGain) {
    switch (controlAxis) {
        case yaw:
            switch (_whichGain) {
                case proportional:
                    _pids.yawPID.setKp(desiredGain);
                case integral:
                    _pids.yawPID.setKi(desiredGain);
                case derivative:
                    _pids.yawPID.setKd(desiredGain);
            }
            break;
        case pitch: 
            switch (_whichGain) {
                case proportional:
                    _pids.pitchPID.setKp(desiredGain);
                case integral:
                    _pids.pitchPID.setKi(desiredGain);
                case derivative:
                    _pids.pitchPID.setKd(desiredGain);
            }
            break;
        case roll:
            switch (_whichGain) {
                case proportional:
                    _pids.rollPID.setKp(desiredGain);
                case integral:
                    _pids.rollPID.setKi(desiredGain);
                case derivative:
                    _pids.rollPID.setKd(desiredGain);
            }
            break;
        case throttle:
            switch (_whichGain) {
                case proportional:
                    _pids.thrustPID.setKp(desiredGain);
                case integral:
                    _pids.thrustPID.setKi(desiredGain);
                case derivative:
                    _pids.thrustPID.setKd(desiredGain);
            }
            break;
    }
}  // namespace AM
