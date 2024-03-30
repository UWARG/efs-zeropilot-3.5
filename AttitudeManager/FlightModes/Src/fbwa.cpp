#include "fbwa.hpp"
#include "CommonDataTypes.hpp"
#include "AM.hpp"
#include <cassert>

namespace AM {
    AttitudeManagerInput FBWA::run(const AttitudeManagerInput& input, AxisPIDs _pids) {
        
        AttitudeManagerInput mappedOutputs;

        // TODO: bound checking that doesn't throw an abort() and program exit
        // Bounded checking to make sure input is within max and min
        // assert(input.pitch <= AM::AttitudeManager::INPUT_MAX && input.pitch >= AM::AttitudeManager::INPUT_MIN);
        // assert(input.roll <= AM::AttitudeManager::INPUT_MAX && input.roll >= AM::AttitudeManager::INPUT_MIN);
        // assert(input.yaw <= AM::AttitudeManager::INPUT_MAX && input.yaw >= AM::AttitudeManager::INPUT_MIN);
        // assert(input.throttle <= AM::AttitudeManager::INPUT_MAX && input.throttle >= AM::AttitudeManager::INPUT_MIN);

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
        

        _pids.pitch.setDesired(mappedOutputs.pitch);
        _pids.roll.setDesired(mappedOutputs.roll);
        _pids.yaw.setDesired(mappedOutputs.yaw);

        
        return mappedOutputs;
    }

    void FBWA::updatePid() {} //Needs to be implemented

    void FBWA::updatePidGains(PIDController _axis, GainTerm whichGain, float desiredGain) {
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
    } //Needs to be implemented

    void FBWA::updatePidGains() {} //Needs to be implemented

    void FBWA::updateControlLimits(ControlLimits_t limits) {
        // TODO: make this better than a straight copy
        fbwa_control_limits = limits;
    }
}