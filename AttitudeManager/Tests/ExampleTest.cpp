#include <gtest/gtest.h>
#include "AM.hpp"

// Demonstrate some basic assertions.
TEST(AttitudeManager, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

TEST(AttitudeManager, SetControlInputs) {
    struct AM::AttitudeManagerInput control_inputs = { 
        .roll = 10.0f,
        .pitch = 10.0f,
        .yaw = 10.0f,
        .throttle = 10.0f
    };
    AM::AttitudeManager::setControlInputs(control_inputs);

    struct AM::AttitudeManagerInput stored_control_inputs = AM::AttitudeManager::getControlInputs();
    EXPECT_EQ(control_inputs.roll, stored_control_inputs.roll);
    EXPECT_EQ(control_inputs.pitch, stored_control_inputs.pitch);
    EXPECT_EQ(control_inputs.yaw, stored_control_inputs.yaw);
    EXPECT_EQ(control_inputs.throttle, stored_control_inputs.throttle);
}

TEST(AttitudeManager, GetControlInputs) {
    struct AM::AttitudeManagerInput control_inputs = { 
        .roll = 0.0f,
        .pitch = 0.0f,
        .yaw = 0.0f,
        .throttle = 0.0f
    };
    struct AM::AttitudeManagerInput stored_control_inputs = AM::AttitudeManager::getControlInputs();
    EXPECT_EQ(control_inputs.roll, stored_control_inputs.roll);
    EXPECT_EQ(control_inputs.pitch, stored_control_inputs.pitch);
    EXPECT_EQ(control_inputs.yaw, stored_control_inputs.yaw);
    EXPECT_EQ(control_inputs.throttle, stored_control_inputs.throttle);
}