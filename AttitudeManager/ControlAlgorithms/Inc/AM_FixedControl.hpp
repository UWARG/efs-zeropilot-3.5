/*
 * AM_FixedControl.hpp
 *
 * Attitude Manager Level Mode Quad Controller Algorithm
 *
 * Created on: Oct 22, 2022
 * Author(s): Aidan Bowers, Jack Greenwood
 */
#ifndef ZPSW3_AM_FIXED_CONTROL_HPP
#define ZPSW3_AM_FIXED_CONTROL_HPP

#include "AM_ControlInterface.hpp"
#include "PID.hpp"

namespace AM {

class FixedControl : public ControlInterface {
   public:
    FixedControl(ActuatorConfig &engine, ActuatorConfig &leftAileron, ActuatorConfig &rightAileron,
                 ActuatorConfig &rudder, ActuatorConfig &elevator)
        : ControlInterface(NumActuatorIdx),
          configs{engine, leftAileron, rightAileron, rudder, elevator} {};

    std::vector<ActuatorOutput> runControlsAlgorithm(
        const AttitudeManagerInput &instructions) override;

    void updatePid() override { return; }

   private:
    enum ActuatorIdx {
        Engine = 0,
        LeftAileron,
        RightAileron,
        Rudder,
        Elevator,
        NumActuatorIdx  // Must always be last
    };

    // TODO: Implement different fixed wing controls algorithms
    // class FixedControlAcro : public AM::FixedControl {
    //   public:
    //     void updatePid() override {return;}
    //   private:
    //     typedef struct PIDs {
    //     };
    // };

    // class FixedControlLevel : public AM::FixedControl {
    //   public:
    //     void updatePid() override {return;}
    //   private:
    //       typedef struct PIDs {
    //     };
    // };

    // class FixedControlGps : public AM::FixedControl {
    //   public:
    //     void updatePid() override {return;}
    //   private:
    //       typedef struct PIDs {
    //         };
    // };

    // class FixedControlManual : public AM::FixedControl {
    //     public:
    //         void stickMapping();
    //     private:
    // };

    const ActuatorConfig configs[NumActuatorIdx];

    static constexpr float MAX_BANK_ANGLE = 20;   // Max angle defined in degrees.
    static constexpr float MAX_PITCH_ANGLE = 20;  // Max angle defined in degrees.

    static constexpr int MAX_I_WINDUP = 1;

    static constexpr float BANK_KP = 4.2;
    static constexpr float BANK_KI = 0.0;
    static constexpr float BANK_KD = 0.0;
    static constexpr float BANK_I_WINDUP = 0.0;

    static constexpr float RUDDER_KP = 0.2;
    static constexpr float RUDDER_KI = 0.2;
    static constexpr float RUDDER_KD = 7;
    static constexpr float RUDDER_I_WINDUP = 0.0;

    static constexpr float PITCH_KP = 1.0;
    static constexpr float PITCH_KI = 0.0;
    static constexpr float PITCH_KD = 0.0;
    static constexpr float PITCH_I_WINDUP = 0.0;

    static constexpr float AIRSPEED_KP = 80.0;
    static constexpr float AIRSPEED_KI = 0.0;        // Values pulled from ZPSW1,
    static constexpr float AIRSPEED_KD = 0.0;        //
    static constexpr float AIRSPEED_I_WINDUP = 0.0;  //
    static constexpr float AIRSPEED_MIN = 0.0;       // unsure of units or reason
    static constexpr float AIRSPEED_MAX = 100;

    // Define PID values once to be passed around
    PIDController pid_bank{BANK_KP,       BANK_KI,         BANK_KD,
                           BANK_I_WINDUP, -MAX_BANK_ANGLE, MAX_BANK_ANGLE};
    PIDController pid_rudder{RUDDER_KP, RUDDER_KI, RUDDER_KD, RUDDER_I_WINDUP, -100, 100};
    PIDController pid_pitch{PITCH_KP,       PITCH_KI,         PITCH_KD,
                            PITCH_I_WINDUP, -MAX_PITCH_ANGLE, MAX_PITCH_ANGLE};
    PIDController pid_airspeed{AIRSPEED_KP,       AIRSPEED_KI,  AIRSPEED_KD,
                               AIRSPEED_I_WINDUP, AIRSPEED_MIN, AIRSPEED_MAX};

    float mixPIDs(StateMix actuator, float bank, float pitch, float yaw, float throttle) const;
};
}  // namespace AM

#endif  // ZPSW3_AM_FIXED_CONTROL_HPP
