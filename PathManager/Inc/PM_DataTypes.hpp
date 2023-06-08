#ifndef PM_DATATYPES_HPP
#define PM_DATATYPES_HPP

#include <cstdint>


/********************************************************************
* Enums
********************************************************************/


// Used by PM to determine the stage of flight
enum FlightStage{PREFLIGHT = 0, TAKEOFF, CRUISING, LANDING, LANDED, DISARMED, TELEOP}; 


// Used to specify the type of output
// Where TELEOP_MODE is used by SM to indicate to PM to pass data directly to AM 
enum WaypointType {PATH_FOLLOW = 0, ORBIT_FOLLOW, HOVER_WAYPOINT, TAKEOFF_WAYPOINT, LANDING_WAYPOINT, TRANSITION_WAYPOINT, TELEOP_MODE};

enum ModifyFlightPathCommand { NO_FLIGHT_PATH_EDIT = 0, INITIALIZE_FLIGHT_PATH, APPEND, INSERT, UPDATE, DELETE, NUKE }; // Used by cruisingState
enum GetNextDirectionsCommand { REGULAR_PATH_FOLLOWING = 0, TOGGLE_HOLDING, TOGGLE_HEAD_HOME }; // Used by cruisingState


/********************************************************************
* Important Structs
********************************************************************/


/**
* Structure stores information about the waypoints along our path to the destination and back.
*/
struct WaypointData {
    int waypointId;                   // Id of the waypoint
    WaypointData * next;                 // Next waypoint
    WaypointData * previous;             // Previous waypoint
    long double latitude;             // Latitude of waypoint
    long double longitude;            // Longitude of waypoint
    int altitude;                     // Altitude of waypoint
    WaypointType waypoint_type; 
    double velocity_target; 
};

// Used in Cruising State 
struct WaypointManager_Data_In {
    long double latitude;
    long double longitude;
    int altitude;
    double track;
};

struct WaypointManager_Data_Out{
    float desiredTrack;              // Desired track to stay on path
    int desiredAltitude;                // Desired altitude at next waypoint
    long double distanceToNextWaypoint; // Distance to the next waypoint (helps with airspeed PID)
    float distanceX, distanceY, distanceZ;
    float rotation; 
    //WaypointStatus errorCode;          // Contains error codes
    bool isDataNew;                     // Notifies PID modules if the data in this structure is new
    int desiredAirspeed;                // FUN FACT WE NEED THIS 
    uint32_t timeOfData;                // The time that the data in this structure was collected
    WaypointType waypoint_type;       // Output type (determines which parameters are defined)
};

#endif

