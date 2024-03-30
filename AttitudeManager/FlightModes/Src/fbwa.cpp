#include "fbwa.hpp"
#include "CommonDataTypes.hpp"
#include "AM.hpp"
#include "config.hpp"
#include <cassert>

namespace AM {
    AttitudeManagerInput FBWA::run(const AttitudeManagerInput& input) {
        
        AttitudeManagerInput mappedOutputs;

        assert(AM::AttitudeManager::INPUT_MAX == 100 && AM::AttitudeManager::INPUT_MIN == -100);

        //Importing max/min limits here
        float yawMin = config::flightmodes[1].tuningData.controlLimits.yawLimit.min;
        float yawMax = config::flightmodes[1].tuningData.controlLimits.yawLimit.max;

        float rollMin = config::flightmodes[1].tuningData.controlLimits.rollLimit.min;
        float rollMax = config::flightmodes[1].tuningData.controlLimits.rollLimit.max;
        
        float pitchMin = config::flightmodes[1].tuningData.controlLimits.pitchLimit.min;
        float pitchMax = config::flightmodes[1].tuningData.controlLimits.pitchLimit.max;

        // Mapping inputs to outputs with ratios of min-max
        mappedOutputs.pitch = pitchMin + (input.pitch - AM::AttitudeManager::INPUT_MIN) * (pitchMax - pitchMin) / (AM::AttitudeManager::INPUT_MAX - AM::AttitudeManager::INPUT_MIN);
        mappedOutputs.roll = rollMin + (input.roll - AM::AttitudeManager::INPUT_MIN) * (rollMax - rollMin) / (AM::AttitudeManager::INPUT_MAX - AM::AttitudeManager::INPUT_MIN);
        mappedOutputs.yaw = yawMin + (input.yaw - AM::AttitudeManager::INPUT_MIN) * (yawMax - yawMin) / (AM::AttitudeManager::INPUT_MAX - AM::AttitudeManager::INPUT_MIN);
        mappedOutputs.throttle = input.throttle;

        return mappedOutputs;
    }

    void FBWA::updatePid() {} //Needs to be implemented

    void FBWA::updatePidGains() {} //Needs to be implemented

    void FBWA::updateControlLimits() {} //Needs to be implemented
}