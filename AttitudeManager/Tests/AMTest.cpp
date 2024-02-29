#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "AM.hpp"
#include "CommonDataTypes.hpp"
#include "config_foundation.hpp"
#include "MockChannel.hpp"
#include "MockFlightmode.hpp"

namespace AM {

TEST(AttitudeManagerOutputToMotor, NoMotors) {
    MockFlightmode controlAlgorithm{};
    MotorInstance_t *motorInstances[NUM_CONTROL_AXIS]{nullptr};
    uint8_t numMotorsPerAxis[NUM_CONTROL_AXIS]{0};

    AM::AttitudeManager am{&controlAlgorithm, motorInstances, numMotorsPerAxis};

    am.outputToMotor(throttle, 40);
    am.outputToMotor(pitch, 30);
    am.outputToMotor(roll, 20);
    am.outputToMotor(yaw, 10);
}

TEST(AttitudeManagerOutputToMotor, MotorsOfSameAxis) {
    MockFlightmode controlAlgorithm{};
    MotorInstance_t *motorInstances[NUM_CONTROL_AXIS]{nullptr};
    uint8_t numMotorsPerAxis[NUM_CONTROL_AXIS]{0};

    MockChannel mockChannelReferences[5]{};

    MotorInstance_t pitchMotorReferences[] = {
        {
            .motorInstance = &mockChannelReferences[0],
            .isInverted = false
        },
        {
            .motorInstance = &mockChannelReferences[1],
            .isInverted = false
        },
        {
            .motorInstance = &mockChannelReferences[2],
            .isInverted = false
        },
        {
            .motorInstance = &mockChannelReferences[3],
            .isInverted = false
        },
        {
            .motorInstance = &mockChannelReferences[4],
            .isInverted = false
        }
    };

    motorInstances[pitch] = pitchMotorReferences;

    numMotorsPerAxis[pitch] = 5;

    AM::AttitudeManager am{&controlAlgorithm, motorInstances, numMotorsPerAxis};

    // Setting non-existent motors shouldn't affect yaw
    EXPECT_CALL(mockChannelReferences[0], set(30));
    EXPECT_CALL(mockChannelReferences[1], set(30));
    EXPECT_CALL(mockChannelReferences[2], set(30));
    EXPECT_CALL(mockChannelReferences[3], set(30));
    EXPECT_CALL(mockChannelReferences[4], set(30));

    am.outputToMotor(yaw, 10);
    am.outputToMotor(throttle, 40);
    am.outputToMotor(pitch, 30);
    am.outputToMotor(roll, 20);

    // Should set all of them
    EXPECT_CALL(mockChannelReferences[0], set(60));
    EXPECT_CALL(mockChannelReferences[1], set(60));
    EXPECT_CALL(mockChannelReferences[2], set(60));
    EXPECT_CALL(mockChannelReferences[3], set(60));
    EXPECT_CALL(mockChannelReferences[4], set(60));

    am.outputToMotor(pitch, 60);
}

TEST(AttitudeManagerOutputToMotor, setOutputInRandomAxisOrder) {
    MockFlightmode controlAlgorithm{};
    MotorInstance_t *motorInstances[NUM_CONTROL_AXIS]{nullptr};
    uint8_t numMotorsPerAxis[NUM_CONTROL_AXIS]{0};

    MockChannel mockChannelReferences[4]{};

    MotorInstance_t yawMotorReferences[] = {
        {
            .motorInstance = &mockChannelReferences[0],
            .isInverted = false
        }
    };

    MotorInstance_t pitchMotorReferences[] = {
        {
            .motorInstance = &mockChannelReferences[1],
            .isInverted = false
        }
    };

    MotorInstance_t rollMotorReferences[] = {
        {
            .motorInstance = &mockChannelReferences[2],
            .isInverted = false
        }
    };

    MotorInstance_t throttleMotorReferences[] = {
        {
            .motorInstance = &mockChannelReferences[3],
            .isInverted = false
        }
    };

    motorInstances[yaw] = yawMotorReferences;
    motorInstances[pitch] = pitchMotorReferences;
    motorInstances[roll] = rollMotorReferences;
    motorInstances[throttle] = throttleMotorReferences;

    numMotorsPerAxis[yaw] = 1;
    numMotorsPerAxis[pitch] = 1;
    numMotorsPerAxis[roll] = 1;
    numMotorsPerAxis[throttle] = 1;

    AM::AttitudeManager am{&controlAlgorithm, motorInstances, numMotorsPerAxis};

    // Should invert all inputs
    EXPECT_CALL(mockChannelReferences[0], set(12));
    EXPECT_CALL(mockChannelReferences[1], set(75));
    EXPECT_CALL(mockChannelReferences[2], set(100));
    EXPECT_CALL(mockChannelReferences[3], set(33));

    am.outputToMotor(yaw, 12);
    am.outputToMotor(throttle, 33);
    am.outputToMotor(pitch, 75);
    am.outputToMotor(roll, 100);
}

TEST(AttitudeManagerOutputToMotor, InvertedTest) {
    MockFlightmode controlAlgorithm{};
    MotorInstance_t *motorInstances[NUM_CONTROL_AXIS]{nullptr};
    uint8_t numMotorsPerAxis[NUM_CONTROL_AXIS]{0};

    MockChannel mockChannelReferences[4]{};

    MotorInstance_t yawMotorReferences[] = {
        {
            .motorInstance = &mockChannelReferences[0],
            .isInverted = true
        }
    };

    MotorInstance_t pitchMotorReferences[] = {
        {
            .motorInstance = &mockChannelReferences[1],
            .isInverted = true
        }
    };

    MotorInstance_t rollMotorReferences[] = {
        {
            .motorInstance = &mockChannelReferences[2],
            .isInverted = true
        }
    };

    MotorInstance_t throttleMotorReferences[] = {
        {
            .motorInstance = &mockChannelReferences[3],
            .isInverted = true
        }
    };

    motorInstances[yaw] = yawMotorReferences;
    motorInstances[pitch] = pitchMotorReferences;
    motorInstances[roll] = rollMotorReferences;
    motorInstances[throttle] = throttleMotorReferences;

    numMotorsPerAxis[yaw] = 1;
    numMotorsPerAxis[pitch] = 1;
    numMotorsPerAxis[roll] = 1;
    numMotorsPerAxis[throttle] = 1;

    AM::AttitudeManager am{&controlAlgorithm, motorInstances, numMotorsPerAxis};

    // Should invert all inputs
    EXPECT_CALL(mockChannelReferences[0], set(88));
    EXPECT_CALL(mockChannelReferences[1], set(25));
    EXPECT_CALL(mockChannelReferences[2], set(0));
    EXPECT_CALL(mockChannelReferences[3], set(67));

    am.outputToMotor(yaw, 12);
    am.outputToMotor(pitch, 75);
    am.outputToMotor(roll, 100);
    am.outputToMotor(throttle, 33);
}

TEST(AttitudeManagerOutputToMotor, CombinedTest) {
    MockFlightmode controlAlgorithm{};
    MotorInstance_t *motorInstances[NUM_CONTROL_AXIS]{nullptr};
    uint8_t numMotorsPerAxis[NUM_CONTROL_AXIS]{0};

    MockChannel mockChannelReferences[8]{};

    MotorInstance_t yawMotorReferences[] = {
        {
            .motorInstance = &mockChannelReferences[0],
            .isInverted = false
        },
        {
            .motorInstance = &mockChannelReferences[1],
            .isInverted = false
        }
    };

    MotorInstance_t pitchMotorReferences[] = {
        {
            .motorInstance = &mockChannelReferences[2],
            .isInverted = true
        },
        {
            .motorInstance = &mockChannelReferences[3],
            .isInverted = true
        },
        {
            .motorInstance = &mockChannelReferences[4],
            .isInverted = false
        }
    };

    MotorInstance_t rollMotorReferences[] = {
        {
            .motorInstance = &mockChannelReferences[5],
            .isInverted = false
        }
    };

    MotorInstance_t throttleMotorReferences[] = {
        {
            .motorInstance = &mockChannelReferences[6],
            .isInverted = false
        },
        {
            .motorInstance = &mockChannelReferences[7],
            .isInverted = true
        }
    };

    motorInstances[yaw] = yawMotorReferences;
    motorInstances[pitch] = pitchMotorReferences;
    motorInstances[roll] = rollMotorReferences;
    motorInstances[throttle] = throttleMotorReferences;

    numMotorsPerAxis[yaw] = 2;
    numMotorsPerAxis[pitch] = 3;
    numMotorsPerAxis[roll] = 1;
    numMotorsPerAxis[throttle] = 2;

    AM::AttitudeManager am{&controlAlgorithm, motorInstances, numMotorsPerAxis};

    // Should invert all necessary inputs
    EXPECT_CALL(mockChannelReferences[0], set(84));
    EXPECT_CALL(mockChannelReferences[1], set(84));
    EXPECT_CALL(mockChannelReferences[2], set(73));
    EXPECT_CALL(mockChannelReferences[3], set(73));
    EXPECT_CALL(mockChannelReferences[4], set(27));
    EXPECT_CALL(mockChannelReferences[5], set(94));
    EXPECT_CALL(mockChannelReferences[6], set(5));
    EXPECT_CALL(mockChannelReferences[7], set(95));

    am.outputToMotor(yaw, 84);
    am.outputToMotor(pitch, 27);
    am.outputToMotor(roll, 94);
    am.outputToMotor(throttle, 5);
}

}