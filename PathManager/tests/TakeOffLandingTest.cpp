#include <gtest/gtest.h>
#include "../Inc/PM_LandingTakeoffManager.hpp"

using namespace LOS;
using namespace PM;

// Take off function
TEST(TakeOffFunctions, takeoffSequenceTest) {
    LandingTakeoffManager takeoffManager;
    LosSFData input;
    input.altitude = 0;
    takeoffManager.getTakeoffAltitudeTarget(input.altitude);

    // get first waypoint
    auto first_waypoint = takeoffManager.createTakeoffWaypoint(input);

    // get second waypoint
    input.altitude = 1;
    auto second_waypoint = takeoffManager.createTakeoffWaypoint(input);

    // check values
    EXPECT_NEAR(first_waypoint.speed, 0.027646, 0.000001);
    EXPECT_NEAR(second_waypoint.speed, 0.407766, 0.000001);
}

// Landing function
TEST(LandingFunctions, landingSequenceTest) { 
    LandingTakeoffManager landingManager;
    LosSFData input;
    input.altitude = 0;
    // initialize ground
    landingManager.getTakeoffAltitudeTarget(input.altitude);

    input.altitude = 6;
    landingManager.getLandingAltitudeTarget(input.altitude);

    input.altitude = 5;
    auto first_waypoint = landingManager.createLandingWaypoint(input);

    input.altitude = 1;
    auto second_waypoint = landingManager.createLandingWaypoint(input);

    // check values
    EXPECT_NEAR(first_waypoint.speed, 0.004596, 0.000001);
    EXPECT_NEAR(second_waypoint.speed, 0.549888, 0.000001);
}
