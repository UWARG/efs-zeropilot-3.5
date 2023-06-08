//
// Created by Gordon Fountain 2022-11-30.
//

#ifndef ZPSW3_COMMON_DATATYPES_HPP
#define ZPSW3_COMMON_DATATYPES_HPP

#include <stdint.h>
#include "AM_DataTypes.hpp"

namespace LOS {
// Struct copied from old code
struct LosSFData {
    float roll, pitch, yaw; //rad
    float rollRate, pitchRate, yawRate; //rad/s
    float airspeed; //m/s
    float altitude; //m
    float rateOfClimb; //m/s
    long double latitude; //Decimal degrees
    float latitudeSpeed; //m/s
    long double longitude; //Decimal degrees
    float longitudeSpeed; //m/s
    double track; // degrees
    double heading; //degrees
};

} // Namespace LOS

namespace PM {

struct JetsonToZpMovementCommand {
  float dist_forward;
  float dist_right;
  float dist_up;
  float heading;
};

struct LandingInitiationCommand {
    bool start_landing; 
};

struct JetsonMovementRequest {
    bool request;
};

struct TelemWaypoint {
    uint8_t waypoint_id;
    double longitude;
    double latitude;
    double altitude;
};

struct WaypointsCommand {
    uint8_t num_waypoints; // number of valid waypoints in the list 
    TelemWaypoint waypoints[5]; 
};

struct PM_AM_Commands {
  // heading unit vector and magnitude
  float dist_forward; 
  float dist_right; 
  float dist_up; 
  float magnitude; // Magnitude distance to waypoint target
  float heading; // heading at target waypoint
  double velocity_target; // Target velocity of drone approaching target
}; 

struct SM_PM_Commands {
    WaypointsCommand telemetry_commands;
    JetsonToZpMovementCommand jetson_commands; 
    LandingInitiationCommand landing_initiation; 
    LOS::LosSFData sf_data;
};

struct PM_SM_Commands {
    JetsonMovementRequest jetson_movement_req;
};

} // Namespace PM

namespace AM {

struct AM_SM_Commands {
    bool ok;
};

struct SM_AM_Commands {
    uint8_t motor_outputs[12];
};

struct PIDValues {
    double P;
    double I;
    double D;
    double A;
};

struct PIDControllerValues {
    PIDValues axes[6];
};

struct GroundStationPIDSetCommand {
    uint8_t controller;
    uint8_t axis;
    PIDValues values;
};

struct GroundStationPIDSetResponse {
    uint8_t controller_number;
    PIDControllerValues controller;
};

struct ActuatorConfig {
    uint8_t channel = UINT8_MAX;
    StateMix state_mix;
};

struct AttitudeManagerInput {
    float dist_forward; 
    float dist_right; 
    float dist_up; 
    float magnitude = 0;
    float heading = 0;
    double speed = 0;
};  // TODO: What is the correct name?

} // Namespace AM

namespace SM {

struct GroundStationDisarmCommand {
    bool set_armed;
};

struct GroundStationData {
    uint8_t motor_outputs[12];
    LOS::LosSFData sf_data;
    uint8_t battery_voltages[13];
    uint8_t controller_values[16];
};

struct SM_TM_Commands {
    GroundStationData gspc_data;
    PM::JetsonMovementRequest jetson_movement_req;
    AM::GroundStationPIDSetResponse gspc_pid_set_resp;
};

struct TM_SM_Commands {
    GroundStationDisarmCommand ground_station_disarm_command;
    PM::WaypointsCommand waypoint_commands;
    PM::JetsonToZpMovementCommand jetson_commands; 
    PM::LandingInitiationCommand landing_initiation; 
    AM::GroundStationPIDSetCommand gspc_pid_set_command;
};
} // Namepsace SM


#endif  // ZPSW3_COMMON_DATATYPES_HPP
