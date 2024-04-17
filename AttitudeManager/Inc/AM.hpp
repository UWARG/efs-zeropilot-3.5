/*
 * AM.hpp
 *
 * Attitude Manager Header
 *
 * Created on: Oct 12, 2022
 * Author(s): Anthony (anni) Luo; Dhruv Upadhyay
 */
#ifndef ZPSW3_AM_HPP
#define ZPSW3_AM_HPP

#include "CommonDataTypes.hpp"
#include "config_foundation.hpp"
#include "FreeRTOS.h"
#include "flightmode.hpp"
#include "semphr.h"
#include "model.hpp"
#ifdef TESTING
#include <gtest/gtest_prod.h>
#endif

namespace AM {

typedef struct {
    MotorChannel *motorInstance;
    bool isInverted;
} MotorInstance_t;

class AttitudeManager {
   public:

    //Constants used for mapping values
    static constexpr float INPUT_MAX = 100;
    static constexpr float INPUT_MIN = -100;

    static void setControlInputs(const AttitudeManagerInput& new_control_inputs);

    static AttitudeManagerInput getControlInputs();

    AttitudeManager(Flightmode* controlAlgorithm,  MotorInstance_t *(&motorInstances)[], uint8_t (&numMotorsPerAxis)[]);

    ~AttitudeManager();

    void runControlLoopIteration();

   private:
    #ifdef TESTING
    FRIEND_TEST(AttitudeManager, MotorInitializationAndOutput); // Remove FRIEND_TEST when updating tests with setControlInputs
    FRIEND_TEST(AttitudeManagerOutputToMotor, NoMotors);
    FRIEND_TEST(AttitudeManagerOutputToMotor, MotorsOfSameAxis);
    FRIEND_TEST(AttitudeManagerOutputToMotor, setOutputInRandomAxisOrder);
    FRIEND_TEST(AttitudeManagerOutputToMotor, InvertedTest);
    FRIEND_TEST(AttitudeManagerOutputToMotor, CombinedTest);
    #endif

    AttitudeManager();
    void outputToMotor(ControlAxis_t axis, uint8_t percent);

    static SemaphoreHandle_t control_inputs_mutex;
    static struct AttitudeManagerInput control_inputs;

    Flightmode *controlAlgorithm_;
    MotorInstance_t *(&motorInstances_)[];
    uint8_t (&numMotorsPerAxis_)[];    

    PIDController pitchPID = PIDController(config::flightmodes[1].tuningData.PIDValues.pitchPID.p,
                        config::flightmodes[1].tuningData.PIDValues.pitchPID.i,
                        config::flightmodes[1].tuningData.PIDValues.pitchPID.d, 
                        100, 0, 100, 0, 0, 0);
    PIDController rollPID = PIDController(config::flightmodes[1].tuningData.PIDValues.rollPID.p, 
                        config::flightmodes[1].tuningData.PIDValues.rollPID.i,
                        config::flightmodes[1].tuningData.PIDValues.rollPID.d, 
                        100, 0, 100, 0, 0, 0);
    PIDController yawPID = PIDController(config::flightmodes[1].tuningData.PIDValues.yawPID.p, 
                        config::flightmodes[1].tuningData.PIDValues.yawPID.i, 
                        config::flightmodes[1].tuningData.PIDValues.yawPID.d, 
                        100, 0, 100, 0, 0, 0);
    PIDController throttlePID = PIDController(0, 0, 0, 0, 0, 0, 0, 0, 0);
   };


};  // namespace AM

#endif  // ZPSW3_AM_HPP
