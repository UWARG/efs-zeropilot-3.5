#include <gtest/gtest.h>
#include "AM.hpp"
#include "config.hpp"

TEST(AttitudeManager, MotorInitialization) {
    AM::Flightmode *control_algorithm  = config::flightmodes[0].flightModeConstructor();
    AM::AttitudeManager AM{control_algorithm};
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}
