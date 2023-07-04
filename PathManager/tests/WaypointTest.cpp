#include <gtest/gtest.h>
#include <tuple>
#include <array>
#include "../Inc/PM_Waypoint.hpp"

// void calculate_direction_to_waypoint(float* nextWaypointCoordinates, float* prevWaypointCoordnates, float* waypointDirection);
class WaypointCalculateDirectionTestSuite : public testing::TestWithParam<std::tuple<std::array<float, 2>, std::array<float, 2>, std::array<float, 2>>> {};

INSTANTIATE_TEST_SUITE_P(
  WaypointCalculateDirectionTestParams, WaypointCalculateDirectionTestSuite,
  testing::Values(
    std::make_tuple(std::array<float, 2>{1.0, 1.1}, std::array<float, 2>{1.2, 1.3}, std::array<float, 2>{1.4, 1.5})
  )
);

TEST_P(WaypointCalculateDirectionTestSuite, CalculateDirection) {
  float param1[2], param2[2], expAns[2], actAns[2];
  std::copy(std::begin(std::get<0>(GetParam())), std::end(std::get<0>(GetParam())), std::begin(param1));
  std::copy(std::begin(std::get<1>(GetParam())), std::end(std::get<1>(GetParam())), std::begin(param2));
  std::copy(std::begin(std::get<2>(GetParam())), std::end(std::get<2>(GetParam())), std::begin(expAns));

  PM::Waypoint::calculate_direction_to_waypoint(param1, param2, actAns);

  EXPECT_EQ(actAns[0], expAns[0]);
  EXPECT_EQ(actAns[1], expAns[1]);
}


// float calculate_distance_to_waypoint(float* nextWaypointCoordinates, float* position);
class WaypointCalculateDistanceTestSuite : public testing::TestWithParam<std::tuple<std::array<float, 2>, std::array<float, 2>, float>> {};

INSTANTIATE_TEST_SUITE_P(
  WaypointCalculateDistanceTestParams, WaypointCalculateDistanceTestSuite,
  testing::Values(
    std::make_tuple(std::array<float, 2>{1.0, 1.1}, std::array<float, 2>{1.2, 1.3}, 1.5)
  )
);

TEST_P(WaypointCalculateDistanceTestSuite, CalculateDistance) {
  float param1[2], param2[2], expAns, actAns;
  std::copy(std::begin(std::get<0>(GetParam())), std::end(std::get<0>(GetParam())), std::begin(param1));
  std::copy(std::begin(std::get<1>(GetParam())), std::end(std::get<1>(GetParam())), std::begin(param2));
  expAns = std::get<2>(GetParam());

  actAns = PM::Waypoint::calculate_distance_to_waypoint(param1, param2);

  EXPECT_EQ(actAns, expAns);
}

// float dot_product(float* waypointDirection, float* position, float* halfPlane);
class WaypointDotProductTestSuite : public testing::TestWithParam<std::tuple<std::array<float, 2>, std::array<float, 2>, std::array<float, 2>, float>> {};

INSTANTIATE_TEST_SUITE_P(
  WaypointDotProductTestParams, WaypointDotProductTestSuite,
  testing::Values(
    std::make_tuple(std::array<float, 2>{1.0, 1.1}, std::array<float, 2>{1.2, 1.3}, std::array<float, 2>{1.4, 1.5}, 1.5)
  )
);

TEST_P(WaypointDotProductTestSuite, DotProduct) {
  float param1[2], param2[2], param3[2], expAns, actAns;
  std::copy(std::begin(std::get<0>(GetParam())), std::end(std::get<0>(GetParam())), std::begin(param1));
  std::copy(std::begin(std::get<1>(GetParam())), std::end(std::get<1>(GetParam())), std::begin(param2));
  std::copy(std::begin(std::get<1>(GetParam())), std::end(std::get<1>(GetParam())), std::begin(param3));
  expAns = std::get<3>(GetParam());

  actAns = PM::Waypoint::dot_product(param1, param2, param3);

  EXPECT_EQ(actAns, expAns);
}


// void get_coordinates(long double longitude, long double latitude, float* xyCoordinates);
// - get_coordinates just callse get_distance, so we don't need to unit test this function
// float get_distance(long double lat1, long double lon1, long double lat2, long double lon2);
class WaypointGetDistanceTestSuite : public testing::TestWithParam<std::tuple<long double, long double, long double, long double, float>> {};

INSTANTIATE_TEST_SUITE_P(
  WaypointGetDistanceTestParams, WaypointGetDistanceTestSuite,
  testing::Values(
    std::make_tuple(1.123, 2.234, 3.4232, 4.53423, 1.123124)
  )
);

TEST_P(WaypointGetDistanceTestSuite, GetDistance) {
  long double param1, param2, param3, param4;
  float expAns, actAns;
  std::tie(param1, param2, param3, param4, expAns) = GetParam();

  actAns = PM::Waypoint::get_distance(param1, param2, param3, param4);

  EXPECT_EQ(actAns, expAns);
}


// int follow_orbit(float* position, float* turnCenter, float turnDirection, float turnRadius, float track);
class WaypointFollowOrbitTestSuite : public testing::TestWithParam<std::tuple<std::array<float, 2>, std::array<float, 2>, float, float, float, int>> {};

INSTANTIATE_TEST_SUITE_P(
  WaypointFollowOrbitTestParams, WaypointFollowOrbitTestSuite,
  testing::Values(
    std::make_tuple(std::array<float, 2>{1.1, 1.2}, std::array<float, 2>{1.1, 1.2}, 3.4232, 4.53423, 1.123124, 10)
  )
);

TEST_P(WaypointFollowOrbitTestSuite, FollowOrbit) {
  float param1[2], param2[2], param3, param4, param5;
  int expAns, actAns;
  std::tie(std::ignore, std::ignore, param3, param4, param5, expAns) = GetParam();
  std::copy(std::begin(std::get<0>(GetParam())), std::end(std::get<0>(GetParam())), std::begin(param1));
  std::copy(std::begin(std::get<1>(GetParam())), std::end(std::get<1>(GetParam())), std::begin(param2));

  actAns = PM::Waypoint::follow_orbit(param1, param2, param3, param4, param5);

  EXPECT_EQ(actAns, expAns);
}

// int follow_straight_path(float* waypointDirection, float* targetWaypoint, float* position, float track);
class WaypointFollowStraightTestSuite : public testing::TestWithParam<std::tuple<std::array<float, 2>, std::array<float, 2>, std::array<float, 2>, float, int>> {};

INSTANTIATE_TEST_SUITE_P(
  WaypointFollowStraightTestParams, WaypointFollowStraightTestSuite,
  testing::Values(
    std::make_tuple(std::array<float, 2>{1.1, 1.2}, std::array<float, 2>{1.1, 1.2}, std::array<float, 2>{1.1, 1.2}, 1.123124, 10)
  )
);

TEST_P(WaypointFollowStraightTestSuite, FollowStraight) {
  float param1[2], param2[2], param3[2], param4;
  int expAns, actAns;
  std::tie(std::ignore, std::ignore, std::ignore, param4, expAns) = GetParam();
  std::copy(std::begin(std::get<0>(GetParam())), std::end(std::get<0>(GetParam())), std::begin(param1));
  std::copy(std::begin(std::get<1>(GetParam())), std::end(std::get<1>(GetParam())), std::begin(param2));
  std::copy(std::begin(std::get<2>(GetParam())), std::end(std::get<2>(GetParam())), std::begin(param3));

  actAns = PM::Waypoint::follow_straight_path(param1, param2, param3, param4);

  EXPECT_EQ(actAns, expAns);
}
