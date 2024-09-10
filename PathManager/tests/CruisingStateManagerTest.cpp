#include <gtest/gtest.h>
#include "../Inc/PM_CruisingStateManager.hpp"

#include <iostream>

using namespace LOS;
using namespace PM;

// get next waypoint unit test

// helper functions
bool compare_output(WaypointManager_Data_Out& out1, WaypointManager_Data_Out& out2) {
  std::cout << std::endl;
  std::cout << (out1.desiredAltitude) << " | " << (out2.desiredAltitude) << " | " << (out1.desiredTrack) << " | " << (out2.desiredTrack) << " | "; 
  std::cout << (out1.distanceToNextWaypoint) << " | " << (round(out2.distanceToNextWaypoint)) << " | "  << (out1.waypoint_type) << " | " << (out2.waypoint_type) << " | ";
  std::cout << (out1.distanceX) << " | " << (out2.distanceX) << " | " << (out1.distanceY) << " | " << (out2.distanceY) << " | " << (out1.distanceZ) << " | " << (out2.distanceZ);
  std::cout.flush();

  return (out1.desiredAltitude == out2.desiredAltitude && out1.desiredTrack == out2.desiredTrack && 
          out1.distanceToNextWaypoint == round(out2.distanceToNextWaypoint) && out1.waypoint_type == out2.waypoint_type &&
          out1.distanceX == out2.distanceX && out1.distanceY == out2.distanceY && out1.distanceZ == out2.distanceZ);
}


// next direction unit tests

// Check Desired Track Orbit
TEST(CruisingStateManagerSuite, CruisingStateManagerOrbitTrack) {
  CruisingStateManager cruisingState = CruisingStateManager();

  WaypointManager_Data_Out out1;
  WaypointManager_Data_Out out2;



}


// Check Desired Track Straight 
TEST(CruisingStateManagerSuite, CruisingStateManagerStraightTrack) {
  CruisingStateManager cruisingState = CruisingStateManager();

  WaypointManager_Data_Out out1;
  WaypointManager_Data_Out out2;

  WaypointManager_Data_In input1 = {43.467998128, -80.537331184, 11, 100};
  WaypointManager_Data_In input2 = {43.469649460242174, -80.55044911526599, 34, 86};

    // Stores answers for tests
    WaypointManager_Data_Out ans1;
    ans1.desiredTrack = 294;         
    ans1.desiredAltitude = 33;
    ans1.distanceToNextWaypoint = 960;
    ans1.distanceX = 0;
    ans1.distanceY = 0;
    ans1.distanceZ = 0;
    ans1.isDataNew = true;
    ans1.timeOfData = 0;
    ans1.waypoint_type = PATH_FOLLOW;

    WaypointManager_Data_Out ans2;
    ans2.desiredTrack = 153;        
    ans2.desiredAltitude = 33;
    ans2.distanceToNextWaypoint = 625;
    ans2.distanceX = 0;
    ans2.distanceY = 0;
    ans2.distanceZ = 0;
    ans2.isDataNew = true;
    ans2.timeOfData = 0;
    ans2.waypoint_type = PATH_FOLLOW;

    const int numPaths = 6;
    // Defines flight path
    /*
    Waypoints:
    1. 43.47075830402289, -80.5479053969044
    2. 43.469649460242174, -80.55044911526599
    3. 43.46764349709017, -80.54172626568685
    4. 43.46430420301871, -80.54806720987989
    5. 43.461854997441996, -80.5406705046026
    6. 43.46144872072057, -80.53505945389745
    */
    float latitudes[numPaths] = {43.47075830402289, 43.469649460242174, 43.46764349709017, 43.46430420301871, 43.461854997441996, 43.46144872072057};
    float longitudes[numPaths] = {-80.5479053969044, -80.55044911526599, -80.54172626568685, -80.54806720987989, -80.5406705046026, -80.53505945389745};
    float altitudes[numPaths] = {10, 20, 30, 33, 32, 50};

    WaypointManager_Data_In inputData;
    WaypointsCommand telemetryData;
    telemetryData.num_waypoints = numPaths;
    for (uint8_t i=0; i<numPaths; ++i) {
      telemetryData.waypoints[i] = {
        i, // waypoint_id
        i, // seq_num
        longitudes[i], // longitude
        latitudes[i], // latitude
        altitudes[i] // altitude     
      };    
    }

    auto ret = cruisingState.editFlightPath(&telemetryData, inputData);
    EXPECT_EQ(ret, WaypointStatus::WAYPOINT_SUCCESS);

    auto status1 = cruisingState.pathFollow(input1, &out1);

    auto status2 = cruisingState.pathFollow(input2, &out2);

    std::cout << "Actual Track: " << out1.desiredTrack << "\n" << "Actual Altitude: " << out1.desiredAltitude << "\n" << "Actual distance to next waypoint: " << out1.distanceToNextWaypoint << "\n";
    std::cout << "Desired Track: " << ans1.desiredTrack << "\n" << "Desired Altitude: " << ans1.desiredAltitude << "\n" << "Distance to next waypoint: " << ans1.distanceToNextWaypoint << "\n";

    auto res1 = compare_output(out1, ans1);
    auto res2 = compare_output(out2, ans2);

    // checks
    EXPECT_EQ(status1, WaypointStatus::WAYPOINT_SUCCESS);
    EXPECT_EQ(status2, WaypointStatus::WAYPOINT_SUCCESS);
    EXPECT_TRUE(res1);
    EXPECT_TRUE(res2);
}

// Desired track when next to next waypoint is not defined

// Desired track when next waypoint is not defined




// flight path plan unit tests

// helper functions

bool checkEqual(const TelemWaypoint input1, const WaypointData input2) {
  return (input1.waypoint_id == input2.waypointId && input1.altitude == input2.altitude &&
          input1.latitude == input2.latitude && input1.longitude == input2.longitude);
}

bool createDummyData(WaypointsCommand& telemetryData, uint8_t num_data, double val, bool reset_seq = false) {
  if (num_data > 100) return false;
  for (uint8_t i = 0; i < num_data; ++i) {
    uint8_t seq_num = i;
    if (!reset_seq) seq_num++;
    telemetryData.waypoints[i] = {
      i, // waypoint_id
      seq_num, // seq_num
      val+i, // longitude
      val+i, // latitude
      val+i // altitude     
    };
  }
  telemetryData.num_waypoints = num_data;
  return true;
}

// initialize new flight plan
TEST(CruisingStateManagerSuite, CruisingStateManagerTestFlight) {
  WaypointsCommand telemetryData;
  WaypointManager_Data_In inputData;

  telemetryData.num_waypoints = 4;
  telemetryData.waypoints[0] = {
    0, // waypoint_id
    0, // seq_num
    100, // longitude
    100, // latitude
    100 // altitude
  };
  telemetryData.waypoints[1] = {
    1, // waypoint_id
    1, // seq_num
    101, // longitude
    101, // latitude
    101 // altitude
  };
  telemetryData.waypoints[2] = {
    2, // waypoint_id
    2, // seq_num
    102, // longitude
    102, // latitude
    102 // altitude
  };
  telemetryData.waypoints[3] = {
    3, // waypoint_id
    3, // seq_num
    103, // longitude
    103, // latitude
    103 // altitude
  };

  CruisingStateManager cruisingState = CruisingStateManager();

  auto ret = cruisingState.editFlightPath(&telemetryData, inputData);

  auto waypoint_buffer = cruisingState.get_waypoint_buffer();

  // check waypoint buffer
  EXPECT_TRUE(checkEqual(telemetryData.waypoints[0], *(waypoint_buffer[0])));
  EXPECT_TRUE(checkEqual(telemetryData.waypoints[1], *(waypoint_buffer[1])));
  EXPECT_TRUE(checkEqual(telemetryData.waypoints[2], *(waypoint_buffer[2])));
  EXPECT_TRUE(checkEqual(telemetryData.waypoints[3], *(waypoint_buffer[3])));
}

// overwrite with new flight plan
TEST(CruisingStateManagerSuite, CruisingStateManagerTestNewFlight) {
  // initialize previous flight plan data
  WaypointManager_Data_In inputData;
  CruisingStateManager cruisingState = CruisingStateManager();

  WaypointsCommand dummyTelemData;
  createDummyData(dummyTelemData, 5, 100, true);

  cruisingState.editFlightPath(&dummyTelemData, inputData);  
  
  WaypointsCommand telemetryData;

  telemetryData.num_waypoints = 4;
  telemetryData.waypoints[0] = {
    5, // waypoint_id
    0, // seq_num
    250, // longitude
    250, // latitude
    250 // altitude
  };
  telemetryData.waypoints[1] = {
    6, // waypoint_id
    1, // seq_num
    251, // longitude
    251, // latitude
    251 // altitude
  };
  telemetryData.waypoints[2] = {
    7, // waypoint_id
    2, // seq_num
    252, // longitude
    252, // latitude
    252 // altitude
  };
  telemetryData.waypoints[3] = {
    8, // waypoint_id
    3, // seq_num
    253, // longitude
    253, // latitude
    253 // altitude
  };

  auto ret = cruisingState.editFlightPath(&telemetryData, inputData);

  auto waypoint_buffer = cruisingState.get_waypoint_buffer();
  

  // check waypoint buffer
  EXPECT_EQ(ret, WaypointStatus::WAYPOINT_SUCCESS);
  EXPECT_TRUE(checkEqual(telemetryData.waypoints[0], *(waypoint_buffer[0])));
  EXPECT_TRUE(checkEqual(telemetryData.waypoints[1], *(waypoint_buffer[1])));
  EXPECT_TRUE(checkEqual(telemetryData.waypoints[2], *(waypoint_buffer[2])));
  EXPECT_TRUE(checkEqual(telemetryData.waypoints[3], *(waypoint_buffer[3])));
}

// new flight plan too big
TEST(CruisingStateManagerSuite, CruisingStateManagerTestAppendFlightTooBig) {
  // initialize previous flight plan data
  WaypointManager_Data_In inputData;
  CruisingStateManager cruisingState = CruisingStateManager();

  WaypointsCommand dummyTelemData;
  createDummyData(dummyTelemData, 20, 100, true);
  
  cruisingState.editFlightPath(&dummyTelemData, inputData);  
  
  WaypointsCommand telemetryData;

  createDummyData(telemetryData, 100, 200);

  auto ret = cruisingState.editFlightPath(&telemetryData, inputData);

  // check waypoint buffer
  EXPECT_EQ(ret, WaypointStatus::TOO_MANY_WAYPOINTS);
}

// append waypoint
TEST(CruisingStateManagerSuite, CruisingStateManagerTestAppendFlightPoint) {
  // initialize previous flight plan data
  WaypointManager_Data_In inputData;
  CruisingStateManager cruisingState = CruisingStateManager();

  WaypointsCommand dummyTelemData;
  createDummyData(dummyTelemData, 5, 100, true);

  cruisingState.editFlightPath(&dummyTelemData, inputData);  
  
  WaypointsCommand telemetryData;

  telemetryData.num_waypoints = 4;
  telemetryData.waypoints[0] = {
    5, // waypoint_id
    5, // seq_num
    250, // longitude
    250, // latitude
    250 // altitude
  };
  telemetryData.waypoints[1] = {
    6, // waypoint_id
    6, // seq_num
    251, // longitude
    251, // latitude
    251 // altitude
  };
  telemetryData.waypoints[2] = {
    7, // waypoint_id
    7, // seq_num
    252, // longitude
    252, // latitude
    252 // altitude
  };
  telemetryData.waypoints[3] = {
    8, // waypoint_id
    8, // seq_num
    253, // longitude
    253, // latitude
    253 // altitude
  };

  auto ret = cruisingState.editFlightPath(&telemetryData, inputData);

  auto waypoint_buffer = cruisingState.get_waypoint_buffer();

  // check waypoint buffer
  EXPECT_EQ(ret, WaypointStatus::WAYPOINT_SUCCESS);
  EXPECT_TRUE(checkEqual(telemetryData.waypoints[0], *(waypoint_buffer[5])));
  EXPECT_TRUE(checkEqual(telemetryData.waypoints[1], *(waypoint_buffer[6])));
  EXPECT_TRUE(checkEqual(telemetryData.waypoints[2], *(waypoint_buffer[7])));
  EXPECT_TRUE(checkEqual(telemetryData.waypoints[3], *(waypoint_buffer[8])));
}