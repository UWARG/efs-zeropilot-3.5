#include "manual.hpp"
#include "CommonDataTypes.hpp"
#include <stdexcept>
namespace AM {

AttitudeManagerInput Manual::run(const AttitudeManagerInput& input) {
    return input;
}

void Manual::updatePid() {}
void Manual::updatePidGains(PidAxis pid_axis, GainTerm pid_gain_term, float desired_gain) {
    if ((isnan(pid_gain_term)) || (desired_gain < 0) || (desired_gain > 1)) {
        return;
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
            throw std::invalid_argument("Invalid pid_axis value.");
            break;

    }
}
void Manual::updateControlLimits(ControlLimits_t limits) {}

}  // namespace AM
