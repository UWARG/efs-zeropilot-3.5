#include "constrain.h"
#include "fbwa.hpp"
#include "CommonDataTypes.hpp"
#include "config.hpp"
#include "AM.hpp"

namespace AM {

FBWA::FBWA()
{
    //Get PID tuning data from config
    config::ControlTuning_t tuningData = config::flightmodes[config::FM_FBWA_IDX].tuningData;
    
    PIDController::PID PitchPIDValues = {
        .kp = tuningData.PIDValues.pitchPID.p,
        .kd = tuningData.PIDValues.pitchPID.d,
        .ki = tuningData.PIDValues.pitchPID.i,
        .i_max = tuningData.PIDValues.pitchPID.i_max,
        .min_output = tuningData.PIDValues.pitchPID.min_output,
        .max_output = tuningData.PIDValues.pitchPID.max_output
    };
    
    PIDController::PID RollPIDValues = {
        .kp = tuningData.PIDValues.rollPID.p,
        .kd = tuningData.PIDValues.rollPID.d,
        .ki = tuningData.PIDValues.rollPID.i,
        .i_max = tuningData.PIDValues.rollPID.i_max,
        .min_output = tuningData.PIDValues.rollPID.min_output,
        .max_output = tuningData.PIDValues.rollPID.max_output
    };

    pitchPID_ = new PIDController(PitchPIDValues);
    rollPID_ = new PIDController(RollPIDValues);
}

AttitudeManagerInput FBWA::run(const AttitudeManagerInput& input) {
    // Get latest attitude from sensor fusion
    SensorFusionOutput currentState = AttitudeManager::getSensorFusionData();
    
    //Initialize outputs and get tuning data from config
    AttitudeManagerInput output;
    config::ControlTuning_t tuningData = config::flightmodes[config::FM_FBWA_IDX].tuningData;

    //Apply control limits and convert to degrees
    float targetPitch = constrain(input.pitch, tuningData.controlLimits.pitchLimit.max, tuningData.controlLimits.pitchLimit.min);
    float targetRoll = constrain(input.roll, tuningData.controlLimits.rollLimit.max, tuningData.controlLimits.rollLimit.min);
    targetPitch = AttitudeManager::attitudePercentToDegrees(targetPitch);
    targetRoll = AttitudeManager::attitudePercentToDegrees(targetRoll);

    //Map yaw and throttle directly
    output.yaw = constrain(input.yaw, tuningData.controlLimits.yawLimit.max, tuningData.controlLimits.yawLimit.min);
    output.throttle = constrain(input.yaw, tuningData.controlLimits.thrustLimit.max, tuningData.controlLimits.thrustLimit.min);

    //Execute PID loops for pitch and roll
    if(tuningData.PIDValues.pitchPID.isEnabled)
    {
        output.pitch = pitchPID_->execute(targetPitch, currentState.pitch);
        output.pitch = AttitudeManager::attitudeDegreesToPercent(output.pitch);
    }
    if(tuningData.PIDValues.rollPID.isEnabled)
    {
        output.roll = rollPID_->execute(targetRoll, currentState.roll);
        output.roll = AttitudeManager::attitudeDegreesToPercent(output.roll);
    }

    // Return outputs to motors
    return output;
}

void FBWA::updatePid(ControlAxis_t axis, PIDController::PID newPIDVals)
{
    if(axis == pitch)
    {
        pitchPID_->setNewPid(newPIDVals);
    }
    else if(axis == roll)
    {
        rollPID_->setNewPid(newPIDVals);
    }
}

}  // namespace AM