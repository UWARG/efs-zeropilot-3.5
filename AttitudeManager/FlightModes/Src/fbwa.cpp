#include "fbwa.hpp"
#include "CommonDataTypes.hpp"
#include "AM.hpp"
#include "config.hpp"

namespace AM {
    AttitudeManagerInput FBWA::run(const AttitudeManagerInput& input) {
        
        AttitudeManagerInput mappedOutputs;

        //Importing max/min limits here
        float yawMin = config::flightmodes[1].tuningData.controlLimits.yawLimit.min;
        float yawMax = config::flightmodes[1].tuningData.controlLimits.yawLimit.max;

        float rollMin = config::flightmodes[1].tuningData.controlLimits.rollLimit.min;
        float rollMax = config::flightmodes[1].tuningData.controlLimits.rollLimit.max;
        
        float pitchMin = config::flightmodes[1].tuningData.controlLimits.pitchLimit.min;
        float pitchMax = config::flightmodes[1].tuningData.controlLimits.pitchLimit.max;

        // Mapping inputs to outputs with ratios of min-max
        mappedOutputs.pitch = pitchMin + (input.pitch - pitchMin) * (pitchMax - pitchMin) / (AM::AttitudeManager::INPUT_MAX - AM::AttitudeManager::INPUT_MIN);
        mappedOutputs.roll = rollMin + (input.roll - rollMin) * (rollMax - rollMin) / (AM::AttitudeManager::INPUT_MAX - AM::AttitudeManager::INPUT_MIN);
        mappedOutputs.yaw = yawMin + (input.yaw - yawMin) * (yawMax - yawMin) / (AM::AttitudeManager::INPUT_MAX - AM::AttitudeManager::INPUT_MIN);

        return mappedOutputs;
    }

    void FBWA::updatePid() {} //Needs to be implemented

    void FBWA::updatePidGains() {} //Needs to be implemented
}