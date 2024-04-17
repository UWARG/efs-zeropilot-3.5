#include "manual.hpp"
#include "CommonDataTypes.hpp"
#include <stdexcept>
namespace AM {

AttitudeManagerInput Manual::run(const AttitudeManagerInput& input) {
    return input;
}

void Manual::updatePid() {}
void Manual::updatePidGains(PidAxis pid_axis, GainTerm pid_gain_term, float desired_gain) {

    if ((std::isnan(desired_gain)) || (desired_gain < 0) || (desired_gain > 1)){
        if (desired_gain < 0) {
            desired_gain = 0;
        } else if (desired_gain > 1) {
            desired_gain = 1;
        } else {
            return;
        }
    }

    switch (pid_axis) {
        case PidAxis::Pitch:
            pitchPID.setGainTerm(pid_gain_term, desired_gain);
            break;
        case PidAxis::Roll:
            rollPID.setGainTerm(pid_gain_term, desired_gain);
            break;
        case PidAxis::Yaw:
            yawPID.setGainTerm(pid_gain_term, desired_gain);
            break;
        case PidAxis::Throttle:
            throttlePID.setGainTerm(pid_gain_term, desired_gain);
            break;
        default:
            break;
    }
}
void Manual::updateControlLimits(ControlLimits_t limits) {}

}  // namespace AM
