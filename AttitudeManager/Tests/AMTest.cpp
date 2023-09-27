#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "AM.hpp"

namespace AM {

TEST(AttitudeManager, MotorInitialization) {
    using ::testing::NotNull;

    AM::Flightmode *control_algorithm  = config::flightmodes[0].flightmodeConstructor();
    AM::AttitudeManager am{control_algorithm};

    // Non null references
    EXPECT_THAT(am.motorReferences_[0], NotNull());
    EXPECT_THAT(am.motorReferences_[1], NotNull());
    EXPECT_THAT(am.motorReferences_[2], NotNull());
    EXPECT_THAT(am.motorReferences_[3], NotNull());
    EXPECT_THAT(am.motorReferences_[4], NotNull());

    // Orderly setting by each type
    am.outputToMotor(config::yaw)
    pitch
    roll
    throttle

    // Setting by each type in a random order

    // 

    am.outputToMotor(config::roll, 25);

    // for (int j; j < )
    // am.motorReferences_

    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);

    delete control_algorithm;
}

}
