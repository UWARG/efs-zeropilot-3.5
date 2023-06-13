#include "AM_FixedControl.hpp"

#include "CommonDataTypes.hpp"
#include "PID.hpp"

namespace AM {

std::vector<ActuatorOutput> FixedControl::runControlsAlgorithm(
    const AttitudeManagerInput &instructions) {
    // Current attitude from SF
    LOS::LosSFData current_attitude{};

    // Compute target values
    float target_heading = instructions.heading;
    float target_throttle = instructions.speed;
    float target_pitch = instructions.dist_forward * MAX_PITCH_ANGLE;
    float target_bank = instructions.dist_right * MAX_BANK_ANGLE;

    float bank = pid_bank.execute(target_bank, current_attitude.roll, current_attitude.rollRate);

    float pitch =
        pid_pitch.execute(target_pitch, current_attitude.pitch, current_attitude.pitchRate);

    float yaw =
        pid_rudder.execute(target_heading, current_attitude.heading, current_attitude.yawRate);

    float throttle = pid_airspeed.execute(target_throttle, current_attitude.airspeed);

    // mix the PID's
    float engine_output = mixPIDs(configs[Engine].state_mix, bank, pitch, yaw, throttle);
    float left_aileron_output = mixPIDs(configs[LeftAileron].state_mix, bank, pitch, yaw, throttle);
    float right_aileron_output =
        mixPIDs(configs[RightAileron].state_mix, bank, pitch, yaw, throttle);
    float rudder = mixPIDs(configs[Rudder].state_mix, bank, pitch, yaw, throttle);
    float elevator = mixPIDs(configs[Elevator].state_mix, bank, pitch, yaw, throttle);

    return std::vector<ActuatorOutput>{{configs[Engine].channel, engine_output},
                                       {configs[LeftAileron].channel, left_aileron_output},
                                       {configs[RightAileron].channel, right_aileron_output},
                                       {configs[Rudder].channel, rudder},
                                       {configs[Elevator].channel, elevator}};
}
float FixedControl::mixPIDs(StateMix actuator, float bank, float pitch, float yaw,
                            float throttle) const {
    return constrain<float>(actuator.pitch * pitch + actuator.roll * bank + actuator.yaw * yaw +
                                actuator.velocity_x * throttle,
                            100, 0);
}

// TODO: Directly mapping values from controller to actuators
// void FixedControl::FixedControlManual::stickMapping() {
//     LosLinkRx_t outputs {};
//     LOS_Actuators actuators {};

//     outputs = Los_Link::getInstance().getRx();

//     std::vector<ActuatorOutput> controller_output;
//     for (auto output : controller_output) {
//         int i = 0;
//         actuators.set(output.channel, outputs.rx_channels[i]);
//         i++;
//     }
// }

}  // namespace AM
