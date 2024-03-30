#include "fbwa.hpp"
#include "CommonDataTypes.hpp"
#include "AM.hpp"

namespace AM {
    AttitudeManagerInput FBWA::run(const AttitudeManagerInput& input) {
        
        AttitudeManagerInput mappedOutputs;

        mappedOutputs.pitch = AM::AttitudeManager::PITCH_LIM_MIN + (input.pitch - AM::AttitudeManager::PITCH_LIM_MIN) * (AM::AttitudeManager::PITCH_LIM_MAX - AM::AttitudeManager::PITCH_LIM_MIN) / (AM::AttitudeManager::INPUT_MAX - AM::AttitudeManager::INPUT_MIN);
        AM::AttitudeManager::PITCH_LIM_MAX;

        mappedOutputs.roll = AM::AttitudeManager::ROLL_LIM_MIN + (input.roll - AM::AttitudeManager::ROLL_LIM_MIN) * (AM::AttitudeManager::ROLL_LIM_MAX - AM::AttitudeManager::ROLL_LIM_MIN) / (AM::AttitudeManager::INPUT_MAX - AM::AttitudeManager::INPUT_MIN);
        AM::AttitudeManager::ROLL_LIM_MAX;

        mappedOutputs.yaw = AM::AttitudeManager::YAW_LIM_MIN + (input.yaw - AM::AttitudeManager::YAW_LIM_MIN) * (AM::AttitudeManager::YAW_LIM_MAX - AM::AttitudeManager::YAW_LIM_MIN) / (AM::AttitudeManager::INPUT_MAX - AM::AttitudeManager::INPUT_MIN);
        AM::AttitudeManager::YAW_LIM_MAX;

        return mappedOutputs;
    }

    void FBWA::updatePid() {} //Needs to be implemented

    void FBWA::updatePidGains() {} //Needs to be implemented
}