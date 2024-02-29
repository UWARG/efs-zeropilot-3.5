#include <gtest/gtest.h>

#include "AM.hpp"
#include "manual.hpp"

// Demonstrate some basic assertions.
TEST(AttitudeManager, random_test) {
    AM::MotorInstance_t *motorInstances[NUM_CONTROL_AXIS]{nullptr};
    uint8_t numMotorsPerAxis[NUM_CONTROL_AXIS]{0};
    AM::Flightmode* flightmode = new AM::Manual();
    AM::AttitudeManager am(flightmode, motorInstances, numMotorsPerAxis);

    am.runControlLoopIteration();
}

TEST(ManualMode, check_inputs) {
    AM::Manual manual_mode{};

    AM::AttitudeManagerInput input{
        .roll = 0,
        .pitch = 0,
        .yaw = 0,
        .throttle = 0,
    };
    auto output = manual_mode.run(input);

    EXPECT_EQ(input.roll, output.roll);
    EXPECT_EQ(input.pitch, output.pitch);
    EXPECT_EQ(input.yaw, output.yaw);
    EXPECT_EQ(input.throttle, output.throttle);
}