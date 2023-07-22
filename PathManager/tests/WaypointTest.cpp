#include <gtest/gtest.h>
#include <tuple>
#include <array>
#include "../Inc/PM_Waypoint.hpp"

#define ERROR_RANGE 0.0001

// void calculate_direction_to_waypoint(float* nextWaypointCoordinates, float* prevWaypointCoordnates, float* waypointDirection);
class WaypointCalculateDirectionTestSuite : public testing::TestWithParam<std::tuple<std::array<float, 3>, std::array<float, 3>, std::array<float, 3>>> {};

INSTANTIATE_TEST_SUITE_P(
  WaypointCalculateDirectionTestParams, WaypointCalculateDirectionTestSuite,
  testing::Values(
    std::make_tuple(std::array<float, 3>{0.25, 1.235, 3.125}, std::array<float, 3>{-0.25, 5.025, -1.2}, std::array<float, 3>{0.08662, -0.656581, 0.74926}),
    std::make_tuple(std::array<float, 3>{0, 0, 0}, std::array<float, 3>{4.0, 4.0, 4.0}, std::array<float, 3>{-0.577350, -0.577350, -0.577350}),
    std::make_tuple(std::array<float, 3>{1.0, 2.0, 3.0}, std::array<float, 3>{1.0, 2.0, 0.0}, std::array<float, 3>{0, 0, 1.0})
  )
);

TEST_P(WaypointCalculateDirectionTestSuite, CalculateDirection) {
  float param1[3], param2[3], expAns[3], actAns[3];
  std::copy(std::begin(std::get<0>(GetParam())), std::end(std::get<0>(GetParam())), std::begin(param1));
  std::copy(std::begin(std::get<1>(GetParam())), std::end(std::get<1>(GetParam())), std::begin(param2));
  std::copy(std::begin(std::get<2>(GetParam())), std::end(std::get<2>(GetParam())), std::begin(expAns));

  PM::Waypoint::calculate_direction_to_waypoint(param1, param2, actAns);

  // include errors
  EXPECT_TRUE(abs(actAns[0] - expAns[0]) < ERROR_RANGE);
  EXPECT_TRUE(abs(actAns[1] - expAns[1]) < ERROR_RANGE);
  EXPECT_TRUE(abs(actAns[2] - expAns[2]) < ERROR_RANGE);
}


// float calculate_distance_to_waypoint(float* nextWaypointCoordinates, float* position);
class WaypointCalculateDistanceTestSuite : public testing::TestWithParam<std::tuple<std::array<float, 3>, std::array<float, 3>, float>> {};

INSTANTIATE_TEST_SUITE_P(
  WaypointCalculateDistanceTestParams, WaypointCalculateDistanceTestSuite,
  testing::Values(
    std::make_tuple(std::array<float, 3>{0.25, 1.235, 3.125}, std::array<float, 3>{-0.25, 5.025, -1.2}, 5.772324055352402),
    std::make_tuple(std::array<float, 3>{0, 0, 0}, std::array<float, 3>{4.0, 4.0, 4.0}, 6.92820),
    std::make_tuple(std::array<float, 3>{1.0, 2.0, 3.0}, std::array<float, 3>{1.0, 2.0, 0.0}, 3.0)
  )
);

TEST_P(WaypointCalculateDistanceTestSuite, CalculateDistance) {
  float param1[3], param2[3], expAns, actAns;
  std::copy(std::begin(std::get<0>(GetParam())), std::end(std::get<0>(GetParam())), std::begin(param1));
  std::copy(std::begin(std::get<1>(GetParam())), std::end(std::get<1>(GetParam())), std::begin(param2));
  expAns = std::get<2>(GetParam());

  actAns = PM::Waypoint::calculate_distance_to_waypoint(param1, param2);

  // include errors
  EXPECT_TRUE(abs(actAns - expAns) < ERROR_RANGE);
}

// // float dot_product(float* waypointDirection, float* position, float* halfPlane);
// class WaypointDotProductTestSuite : public testing::TestWithParam<std::tuple<std::array<float, 3>, std::array<float, 3>, std::array<float, 3>, float>> {};

// INSTANTIATE_TEST_SUITE_P(
//   WaypointDotProductTestParams, WaypointDotProductTestSuite,
//   testing::Values(
//     std::make_tuple(std::array<float, 3>{1, 1, 1}, std::array<float, 3>{0, 0, 0}, std::array<float, 3>{}, 1.5)
//   )
// );

// TEST_P(WaypointDotProductTestSuite, DotProduct) {
//   float param1[2], param2[2], param3[2], expAns, actAns;
//   std::copy(std::begin(std::get<0>(GetParam())), std::end(std::get<0>(GetParam())), std::begin(param1));
//   std::copy(std::begin(std::get<1>(GetParam())), std::end(std::get<1>(GetParam())), std::begin(param2));
//   std::copy(std::begin(std::get<2>(GetParam())), std::end(std::get<2>(GetParam())), std::begin(param3));
//   expAns = std::get<3>(GetParam());

//   actAns = PM::Waypoint::dot_product(param1, param2, param3);

//   EXPECT_EQ(actAns, expAns);
// }


// void get_coordinates(long double longitude, long double latitude, float* xyCoordinates);
// - get_coordinates just callse get_distance, so we don't need to unit test this function
// float get_distance(long double lat1, long double lon1, long double lat2, long double lon2);
class WaypointGetDistanceTestSuite : public testing::TestWithParam<std::tuple<long double, long double, long double, long double, float>> {};

INSTANTIATE_TEST_SUITE_P(
  WaypointGetDistanceTestParams, WaypointGetDistanceTestSuite,
  testing::Values(
    std::make_tuple(1.123, -2.234, 1.123, -2.234, 0),
    std::make_tuple(10, -10, 10.5, -10.5, -78002)
  )
);

TEST_P(WaypointGetDistanceTestSuite, GetDistance) {
  long double param1, param2, param3, param4;
  float expAns, actAns;
  std::tie(param1, param2, param3, param4, expAns) = GetParam();

  actAns = PM::Waypoint::get_distance(param1, param2, param3, param4);

  EXPECT_TRUE(abs(actAns - expAns) < 100);
}


// int follow_orbit(float* position, float* turnCenter, float turnDirection, float turnRadius, float track);
class WaypointFollowOrbitTestSuite : public testing::TestWithParam<std::tuple<std::array<float, 2>, std::array<float, 2>, float, float, float, int>> {};

INSTANTIATE_TEST_SUITE_P(
  WaypointFollowOrbitTestParams, WaypointFollowOrbitTestSuite,
  testing::Values(
    std::make_tuple(std::array<float, 2>{0.0, 1.0}, std::array<float, 2>{0.0, 0.0}, 1, 1, 265, 270)
  )
);

TEST_P(WaypointFollowOrbitTestSuite, FollowOrbit) {
  float param1[2], param2[2], param3, param4, param5;
  int expAns, actAns;
  std::tie(std::ignore, std::ignore, param3, param4, param5, expAns) = GetParam();
  std::copy(std::begin(std::get<0>(GetParam())), std::end(std::get<0>(GetParam())), std::begin(param1));
  std::copy(std::begin(std::get<1>(GetParam())), std::end(std::get<1>(GetParam())), std::begin(param2));

  actAns = PM::Waypoint::follow_orbit(param1, param2, param3, param4, param5);

  EXPECT_TRUE(abs(actAns - expAns) < 1);
}

// int follow_straight_path(float* waypointDirection, float* targetWaypoint, float* position, float track);
class WaypointFollowStraightTestSuite : public testing::TestWithParam<std::tuple<std::array<float, 2>, std::array<float, 2>, std::array<float, 2>, float, int>> {};

INSTANTIATE_TEST_SUITE_P(
  WaypointFollowStraightTestParams, WaypointFollowStraightTestSuite,
  testing::Values(
    std::make_tuple(std::array<float, 2>{0.57735, -0.57735}, std::array<float, 2>{10, -10}, std::array<float, 2>{2, -2}, 90, 135),
    std::make_tuple(std::array<float, 2>{0.5, 0.866}, std::array<float, 2>{2, 4}, std::array<float, 2>{0, 0}, 3.43568, 30)
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

  EXPECT_TRUE(abs(actAns - expAns) < 1);
}
