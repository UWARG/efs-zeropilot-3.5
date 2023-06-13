/*
 * AM_LevelQuadControl.hpp
 *
 * Attitude Manager Level Mode Quad Controller Algorithm
 *
 * Created on: Oct 22, 2022
 * Author(s): Aidan Bowers
 */
#ifndef ZPSW3_AM_LEVEL_QUAD_CONTROL_HPP
#define ZPSW3_AM_LEVEL_QUAD_CONTROL_HPP

#include "AM_ControlInterface.hpp"
#include "PID.hpp"

namespace AM {

class LevelQuadControl : public ControlInterface {
   public:
    LevelQuadControl(ActuatorConfig &frontRight, ActuatorConfig &frontLeft,
                     ActuatorConfig &backRight, ActuatorConfig &backLeft)
        : ControlInterface(NumActuatorIdx), configs{frontRight, frontLeft, backRight, backLeft} {};

    std::vector<ActuatorOutput> runControlsAlgorithm(
        const AttitudeManagerInput &instructions) override;

    void updatePid() override { return; }

   private:
    enum ActuatorIdx {
        FrontRight = 0,
        FrontLeft,
        BackRight,
        BackLeft,
        NumActuatorIdx  // Must always be last
    };

    ActuatorConfig configs[NumActuatorIdx];

    static constexpr float max_roll_deg = 60;
    static constexpr float max_pitch_deg = 60;

    static constexpr int pid_abs_max = 100;
    // ? not sure if we need specifics for each angle
    static constexpr int max_i_windup = 1;

    static constexpr float roll_kp = 0.05;
    static constexpr float roll_ki = 0.001;
    static constexpr float roll_kd = 0.015;

    static constexpr float pitch_kp = 0.05;
    static constexpr float pitch_ki = 0.001;
    static constexpr float pitch_kd = 0.015;

    static constexpr float yaw_kp = 0.25;
    static constexpr float yaw_ki = 0.01;
    static constexpr float yaw_kd = 0.05;

    static constexpr float velocity_z_kp = 0.25;
    static constexpr float velocity_z_ki = 0.01;
    static constexpr float velocity_z_kd = 0.05;

    PIDController pid_pitch{pitch_kp, pitch_ki, pitch_kd, max_i_windup, -pid_abs_max, pid_abs_max};
    PIDController pid_roll{roll_kp, roll_ki, roll_kd, max_i_windup, -pid_abs_max, pid_abs_max};
    PIDController pid_yaw{yaw_kp, yaw_ki, yaw_kd, max_i_windup, -pid_abs_max, pid_abs_max};
    PIDController pid_z{velocity_z_kp, velocity_z_ki, velocity_z_kd,
                        max_i_windup,  -pid_abs_max,  pid_abs_max};

    float mixPIDs(StateMix actuator, float roll, float pitch, float yaw, float velocity_z) const;
};
}  // namespace AM

#endif  // ZPSW3_AM_LEVEL_QUAD_CONTROL_HPP
