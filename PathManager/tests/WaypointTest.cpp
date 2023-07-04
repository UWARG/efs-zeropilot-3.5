#include <gtest/gtest.h>
#include <tuple>
#include <array>
#include "../Inc/PM_Waypoint.hpp"

// PowerShell -ExecutionPolicy Bypass

class WaypointTestSuite : public testing::TestWithParam<std::tuple<int, int, int>> {};

INSTANTIATE_TEST_SUITE_P(
  WaypointTestParams, WaypointTestSuite,
  testing::Values(
    std::make_tuple(1, 2, 2),
    std::make_tuple(4, 5, 20),
    std::make_tuple(-2, 300, -600)
  )
);

// Demonstrate some basic assertions.
TEST_P(WaypointTestSuite, WaypointTest) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  int param1, param2, expAns;
  std::tie(param1, param2, expAns) = GetParam();
  EXPECT_EQ(param1 * param2, expAns);
}

// float calculate_direction_to_waypoint(float* nextWaypointCoordinates, float* prevWaypointCoordnates, float* waypointDirection);
class WaypointCalculateDirectionTestSuite : public testing::TestWithParam<std::tuple<std::array<float, 2>, std::array<float, 2>, std::array<float, 2>>> {};

INSTANTIATE_TEST_SUITE_P(
  WaypointCalculateDirectionTestParams, WaypointCalculateDirectionTestSuite,
  testing::Values(
    std::make_tuple(std::array<float, 2>{1.0, 1.1}, std::array<float, 2>{1.2, 1.3}, std::array<float, 2>{1.4, 1.5})
  )
);

TEST_P(WaypointCalculateDirectionTestSuite, CalculateDirection) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  float param1[2], param2[2], expAns[2], actAns[2];
  std::copy(std::begin(std::get<0>(GetParam())), std::end(std::get<0>(GetParam())), std::begin(param1));
  std::copy(std::begin(std::get<1>(GetParam())), std::end(std::get<1>(GetParam())), std::begin(param2));
  std::copy(std::begin(std::get<2>(GetParam())), std::end(std::get<2>(GetParam())), std::begin(expAns));

  PM::Waypoint::calculate_direction_to_waypoint(param1, param2, actAns);

  EXPECT_EQ(actAns[0], expAns[0]);
  EXPECT_EQ(actAns[1], expAns[1]);
}


// float calculate_distance_to_waypoint(float* nextWaypointCoordinates, float* position);


// float dot_product(float* waypointDirection, float* position, float* halfPlane);


// void get_coordinates(long double longitude, long double latitude, float* xyCoordinates);


// float get_distance(long double lat1, long double lon1, long double lat2, long double lon2);


// int follow_orbit(float* position, float* turnCenter, float turnDirection, float turnRadius, float track);
    

// int follow_straight_path(float* waypointDirection, float* targetWaypoint, float* position, float track);
