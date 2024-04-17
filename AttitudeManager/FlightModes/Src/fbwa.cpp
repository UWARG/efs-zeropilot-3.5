#include "fbwa.hpp"
#include "CommonDataTypes.hpp"
#include "AM.hpp"
#include <cassert>
#include <stdexcept>

namespace AM {
AttitudeManagerInput FBWA::run(const AttitudeManagerInput& input) {
    AttitudeManagerInput mappedOutputs;

    // TODO: bound checking that doesn't throw an abort() and program exit
    // Bounded checking to make sure input is within max and min
    // assert(input.pitch <= AM::AttitudeManager::INPUT_MAX && input.pitch >=
    // AM::AttitudeManager::INPUT_MIN); assert(input.roll <= AM::AttitudeManager::INPUT_MAX &&
    // input.roll >= AM::AttitudeManager::INPUT_MIN); assert(input.yaw <=
    // AM::AttitudeManager::INPUT_MAX && input.yaw >= AM::AttitudeManager::INPUT_MIN);
    // assert(input.throttle <= AM::AttitudeManager::INPUT_MAX && input.throttle >=
    // AM::AttitudeManager::INPUT_MIN);

    // Mapping inputs to outputs with ratios of min-max
    // Mapping maintains 0->0 I/O
    mappedOutputs.pitch =
        input.pitch < 0
            ? input.pitch * (fbwa_control_limits.pitchLimit.min / AM::AttitudeManager::INPUT_MIN)
            : input.pitch * (fbwa_control_limits.pitchLimit.max / AM::AttitudeManager::INPUT_MAX);
    mappedOutputs.roll =
        input.roll < 0
            ? input.roll * (fbwa_control_limits.rollLimit.min / AM::AttitudeManager::INPUT_MIN)
            : input.roll * (fbwa_control_limits.rollLimit.max / AM::AttitudeManager::INPUT_MAX);
    mappedOutputs.yaw =
        input.yaw < 0
            ? input.yaw * (fbwa_control_limits.yawLimit.min / AM::AttitudeManager::INPUT_MIN)
            : input.yaw * (fbwa_control_limits.yawLimit.max / AM::AttitudeManager::INPUT_MAX);

    // Throttle maps -100-100 to range 0-100
    mappedOutputs.throttle =
        fbwa_control_limits.throttleLimit.min +
        (input.throttle - AM::AttitudeManager::INPUT_MIN) *
            (fbwa_control_limits.throttleLimit.max - fbwa_control_limits.throttleLimit.min) /
            (AM::AttitudeManager::INPUT_MAX - AM::AttitudeManager::INPUT_MIN);
        

    // Need to get actual and actualRate data from IMU filter
    //  pidRoll.execute(mappedOutputs.roll, actual, actualRate);
    //  pidPitch.execute(mappedOutputs.pitch, actual, actualRate);
    //  pidYaw.execute(mappedOutputs.yaw, actual, actualRate);

        return mappedOutputs;
}

void FBWA::updatePid() {}  // Needs to be implemented

void FBWA::updatePidGains(PidAxis pid_axis, GainTerm pid_gain_term, float desired_gain) {

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
            pidPitch.setGainTerm(pid_gain_term, desired_gain);
            break;
        case PidAxis::Roll:
            pidRoll.setGainTerm(pid_gain_term, desired_gain);
            break;
        case PidAxis::Yaw:
            pidYaw.setGainTerm(pid_gain_term, desired_gain);
            break;
        case PidAxis::Throttle:
            break;
        default:
            break;
        } 
}

void FBWA::updateControlLimits(ControlLimits_t limits) {
    // TODO: make this better than a straight copy
    fbwa_control_limits = limits;
}
}  // namespace AM