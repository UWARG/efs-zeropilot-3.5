#include "fbwa.hpp"
#include "CommonDataTypes.hpp"
#include "config.hpp"

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
    //TODO: Get latest attitude from sensor fusion
    float currentPitch = 0;
    float currentRoll = 0;

    //Initialize PID outputs
    float outputPitch = 0;
    float outputRoll = 0;
    
    config::ControlTuning_t tuningData = config::flightmodes[config::FM_FBWA_IDX].tuningData;

    //TODO: Apply control limits

    //Execute PID loops
    if(tuningData.PIDValues.pitchPID.isEnabled)
    {
        outputPitch = pitchPID_->execute(input.pitch, currentPitch);
    }
    if(tuningData.PIDValues.rollPID.isEnabled)
    {
        outputRoll = rollPID_->execute(input.roll, currentRoll);
    }

    //TODO: Return, save, or send outputs to motors
    return input;
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