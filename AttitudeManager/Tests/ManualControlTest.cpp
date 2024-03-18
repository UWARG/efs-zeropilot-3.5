#include "manual.hpp"
#include "AM.hpp"

#include <gtest/gtest.h>

class ManualModeTestFixture : public ::testing::TestWithParam<AM::AttitudeManagerInput>{};

TEST_P(ManualModeTestFixture, check_inputs_are_expected) {
    auto input = GetParam();

    AM::Manual flightmode;
    auto output = flightmode.run(input);

    EXPECT_EQ(input.roll, output.roll);
    EXPECT_EQ(input.pitch, output.pitch);
    EXPECT_EQ(input.yaw, output.yaw);
    EXPECT_EQ(input.throttle, output.throttle);
}

INSTANTIATE_TEST_CASE_P(
    ManualModeTests,
    ManualModeTestFixture,
    ::testing::Values(
        AM::AttitudeManagerInput{.roll = 0, .pitch = 0, .yaw = 0, .throttle = 0 },
        AM::AttitudeManagerInput{.roll = 0xDEAD, .pitch = 0xBEEF, .yaw = 0xFEED, .throttle = 0xFACE},
        AM::AttitudeManagerInput{.roll = 0.1f, .pitch = 0.5f, .yaw = 100, .throttle = 0.001f},
        AM::AttitudeManagerInput{.roll = 10, .pitch = -5.5, .yaw = -66.99, .throttle = -0xFFFF}
    )
);
