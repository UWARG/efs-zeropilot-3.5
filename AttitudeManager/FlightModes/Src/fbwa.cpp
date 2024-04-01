#include "fbwa.hpp"
#include "CommonDataTypes.hpp"
#include "AM.hpp"
#include <cassert>

namespace AM {
    AttitudeManagerInput FBWA::run(const AttitudeManagerInput& input) {
        
        AttitudeManagerInput mappedOutputs;

        // Bounded checking to make sure input is within max and min
        assert(input.pitch <= AM::AttitudeManager::INPUT_MAX && input.pitch >= AM::AttitudeManager::INPUT_MIN);
        assert(input.roll <= AM::AttitudeManager::INPUT_MAX && input.roll >= AM::AttitudeManager::INPUT_MIN);
        assert(input.yaw <= AM::AttitudeManager::INPUT_MAX && input.yaw >= AM::AttitudeManager::INPUT_MIN);
        assert(input.throttle <= AM::AttitudeManager::INPUT_MAX && input.throttle >= AM::AttitudeManager::INPUT_MIN);

        // Mapping inputs to outputs with ratios of min-max
        // Mapping maintains 0->0 I/O
        mappedOutputs.pitch = input.pitch < 0 ? input.pitch * (pitchMin /  AM::AttitudeManager::INPUT_MIN) : input.pitch * (pitchMax /  AM::AttitudeManager::INPUT_MAX);        
        mappedOutputs.roll = input.roll < 0 ? input.roll * (rollMin /  AM::AttitudeManager::INPUT_MIN) : input.roll * (rollMax /  AM::AttitudeManager::INPUT_MAX);        
        mappedOutputs.yaw = input.yaw < 0 ? input.yaw * (yawMin /  AM::AttitudeManager::INPUT_MIN) : input.yaw * (yawMax /  AM::AttitudeManager::INPUT_MAX);

        // Throttle maps -100-100 to range 0-100
        mappedOutputs.throttle = throttleMin + (input.throttle - AM::AttitudeManager::INPUT_MIN) * (throttleMax - throttleMin) / (AM::AttitudeManager::INPUT_MAX - AM::AttitudeManager::INPUT_MIN);

        return mappedOutputs;
    }

    void FBWA::updatePid() {} //Needs to be implemented

    void FBWA::updatePidGains() {} //Needs to be implemented

    void FBWA::updateControlLimits(ControlLimits_t limits) {
        //Importing max/min limits here from config.hpp
        float yawMin = limits.yawLimit.min;
        float yawMax = limits.yawLimit.max;

        float rollMin = limits.rollLimit.min;
        float rollMax = limits.rollLimit.max;
        
        float pitchMin = limits.pitchLimit.min;
        float pitchMax = limits.pitchLimit.max;

        float throttleMin = limits.throttleLimit.min;
        float throttleMax = limits.throttleLimit.max;
    } //Needs to be implemented
}