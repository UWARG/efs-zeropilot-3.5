#ifndef PM_CRUISING_STATE_MANAGER_HPP
#define PM_CRUISING_STATE_MANAGER_HPP

#define MAX_PATH_BUFFER_SIZE 100

#include "PM_DataTypes.hpp"
#include "PM_Waypoint.hpp"
#include "CommonDataTypes.hpp"

namespace PM {

    class CruisingStateManager {
        public:

            CruisingStateManager();
            ~CruisingStateManager();

            /**
             * Function performs any requested modifications on the flight path including appending, inserting, deleting, updating, initializing, and nuking
             *
             * @param telemetryData        --> contains the most recent commands from telemetry
             *
             * @return error code indicating success of operation WaypointStatus
             */
            WaypointStatus editFlightPath(WaypointsCommand * telemetryData, const WaypointManager_Data_In &inputData, bool reset);

            /**
             * Function retrieves the next desired path for the aircraft
             *
             * @param input                --> struct contains inputs required to calculate the desired path for the aircraft
             * @param output               --> struct collects the output data from the Waypoint Manager for use in other states
             *
             * @return error code indicating success of operation WaypointStatus
             */
            WaypointStatus pathFollow(const WaypointManager_Data_In& input, WaypointManager_Data_Out*  output);


            WaypointData ** get_waypoint_buffer();

        private:
            //Stores waypoints
            WaypointData * waypointBuffer[MAX_PATH_BUFFER_SIZE]; //Stores all waypoints
            // WaypointBufferStatus waypointBufferStatus[MAX_PATH_BUFFER_SIZE] = {FREE}; //Keeps status of elements in waypointBuffer array

            // bool newFlightPlan;

            int numWaypoints;
            // int nextFilledIndex; // Index of the array that will be initialized next
            int nextAssignedId;  // ID of the next waypoint that will be initialized
            int currentIndex;    // Index for the waypoint in our flight path we are currently on (If we are going from waypint A and B, currentIndex is the index of waypoint A)
                                 // If current index is -1. it is going from current location to first waypoint
            int orbitPathStatus; // Are we orbiting or following a straight path

            //Home base
            // WaypointData * homeBase;

            // Relative lat and long for coordinate calcilation
            float relativeLongitude;
            float relativeLatitude;

            //Data that will be transferred
            uint16_t desiredTrack;
            int desiredAltitude;
            long double distanceToNextWaypoint;
            float direction[3];
            WaypointStatus errorCode;
            bool dataIsNew;
            WaypointType outputType;

            //Status variables
            // bool goingHome;     // This is set to true when the head_home() function is called.
            // _WaypointStatus errorStatus;
            bool inHold; // Set to true when start_circling() is called
            float turnCenter[3];
            int turnDesiredAltitude;
            int turnDirection; // 1 for CW, 2 for CCW
            float turnRadius;


            /* FLIGHT PATH */
            WaypointData* initialize_waypoint();

            WaypointData* initialize_waypoint(int id, long double longitude, long double latitude, int altitude/*, WaypointType waypointType*/);

            WaypointData* initialize_waypoint(const TelemWaypoint& waypointData /*, WaypointType waypointType*/);

            // WaypointData* initialize_waypoint(long double longitude, long double latitude, int altitude, float turnRadius);

            // WaypointData* initialize_waypoint(const TelemWaypoint& waypointData, WaypointType waypointType, float turnRadius);

            WaypointStatus initialize_flight_plan(WaypointData ** waypoints, uint8_t num);                // Initialize flight plan (waypointBuffer)
            WaypointStatus clear_flight_plan();                                                           // Clear flight plan (waypointBuffer)

            void destroy_waypoint(WaypointData* waypoint);                         // Removes waypoint from heap
            WaypointStatus append_waypoint(WaypointData* newWaypoint);              // Adds a waypoint to the first free element in the waypointBuffer

            /* NEXT DIRECTION */
            WaypointStatus get_next_directions(WaypointManager_Data_In currentStatus, WaypointManager_Data_Out *Data);

            // WaypointStatus start_circling(WaypointManager_Data_In currentStatus, float radius, int direction, int altitude, bool cancelTurning);

            void follow_waypoints(WaypointData* currentWaypoint, float* position, float track);

            // HeadHomeStatus head_home(bool startHeadingHome);

            // void follow_hold_pattern(float* position, float track);

            void next_waypoints(WaypointData* currentWaypoint, float* position, float track);
            void follow_line_segment(WaypointData* currentWaypoint, float* position, float track);
            void follow_last_line_segment(WaypointData* currentWaypoint, float* position, float track);

            void update_return_data(WaypointManager_Data_Out *Data);
    };

} // namespace PM

#endif
