#include "PM_CruisingStateManager.hpp"


namespace PM {

    CruisingStateManager::CruisingStateManager()
    {
        // Initializes important array and id navigation constants
        currentIndex = -1;
        nextAssignedId = 0;
        numWaypoints = 0;

        // Sets relative long and lat
        relativeLongitude = 0;
        relativeLatitude = 0;

        // Sets boolean variables
        inHold = false;
        // goingHome = false;
        dataIsNew = false;
        orbitPathStatus = PATH_FOLLOW;
        errorCode = WAYPOINT_SUCCESS;

        // Initialize all other parameters (defaults)
        desiredTrack = 0;
        desiredAltitude = 0;
        distanceToNextWaypoint = 0.0;
        for (int i=0; i<3; ++i)
        {
            direction[i] = 0.0;
        }
        errorCode = WAYPOINT_SUCCESS;
        dataIsNew = false;
        outputType = PATH_FOLLOW;
        turnDesiredAltitude = 0;
        turnDirection = 0; // 1 for CW, 2 for CCW
        turnRadius = 0.0;

        // Sets empty elements to null to prevent segmentation faults
        for(int i = 0; i < MAX_PATH_BUFFER_SIZE; i++) {
            waypointBuffer[i] = nullptr;
        }
    }

    CruisingStateManager::~CruisingStateManager()
    {
        clear_flight_plan();
    }

    WaypointStatus CruisingStateManager::pathFollow(const WaypointManager_Data_In& input, WaypointManager_Data_Out*  output)
    {
        // if regular path follow
        return get_next_directions(input, output);
    }

    WaypointStatus CruisingStateManager::editFlightPath(WaypointsCommand * telemetryData, const WaypointManager_Data_In &inputData, bool reset)
    {
        if (telemetryData->num_waypoints == 0)
        {
            return INVALID_PARAMETERS;
        }
        
        // initialize waypoints
        WaypointData * Waypoints[MAX_PATH_BUFFER_SIZE];
        for (int i=0; i<telemetryData->num_waypoints; ++i)
        {
            Waypoints[i] = initialize_waypoint(telemetryData->waypoints[i]);
        }

        if (reset)
        {
            clear_flight_plan();
            initialize_flight_plan(Waypoints, telemetryData->num_waypoints);
        }
        else
        {
            for (int i=0; i<telemetryData->num_waypoints; ++i)
            {
                append_waypoint(Waypoints[i]);
            }
        }
    }

    WaypointData ** CruisingStateManager::get_waypoint_buffer() {
        return waypointBuffer;
    }

    WaypointData* CruisingStateManager::initialize_waypoint()
    {
        WaypointData *data = new WaypointData();
        data->waypointId = nextAssignedId++;
        data->next = nullptr;
        data->previous = nullptr;
        data->altitude = 0;
        data->latitude = 0;
        data->longitude = 0;
        data->velocity_target = 0;
        data->waypoint_type = PATH_FOLLOW;
        return data;
    }

    WaypointData* CruisingStateManager::initialize_waypoint(int id, long double longitude, long double latitude, int altitude/*, WaypointType waypointType*/)
    {
        WaypointData *data = new WaypointData();
        data->waypointId = id;
        data->next = nullptr;
        data->previous = nullptr;
        data->altitude = altitude;
        data->latitude = latitude;
        data->longitude = longitude;
        data->velocity_target = 0;
        data->waypoint_type = PATH_FOLLOW;
        return data;
    }

    WaypointData* CruisingStateManager::initialize_waypoint(const TelemWaypoint& waypointData /*, WaypointType waypointType*/)
    {
        return initialize_waypoint(waypointData.waypoint_id, waypointData.longitude, waypointData.latitude, waypointData.altitude);
    }

    WaypointStatus CruisingStateManager::initialize_flight_plan(WaypointData ** waypoints, uint8_t numberOfWaypoints)
    {
        errorCode = WAYPOINT_SUCCESS; 

        // The waypointBuffer array must be empty before we initialize the flight path
        if (numWaypoints != 0) {
            errorCode = UNDEFINED_FAILURE;
            return errorCode;
        }

        // If user passes in too many waypoints, the enum will notify them, but the flight path will be set with the maximum amount of waypoints allowed 
        if (numberOfWaypoints > MAX_PATH_BUFFER_SIZE) {
            errorCode = TOO_MANY_WAYPOINTS;
            numberOfWaypoints = MAX_PATH_BUFFER_SIZE;
        }

        // Initializes the waypointBuffer array
        for (int i = 0; i < numWaypoints; i++) {
            waypointBuffer[i] = waypoints[i]; // Sets the element in the waypointBuffer
        }

        // Links waypoints together
        for (int i = 0; i < numWaypoints; i++) {
            if (i == 0) { // If first waypoint, link to next one only
                waypointBuffer[i]->next = waypointBuffer[i+1];
                waypointBuffer[i]->previous = nullptr;
            } else if (i == numWaypoints - 1) { // If last waypoint, link to previous one only
                waypointBuffer[i]->next = nullptr;
                waypointBuffer[i]->previous = waypointBuffer[i-1];
            } else {
                waypointBuffer[i]->next = waypointBuffer[i+1];
                waypointBuffer[i]->previous = waypointBuffer[i-1];
            }
        }

        numWaypoints = numberOfWaypoints;
        return errorCode;
    }

    void CruisingStateManager::destroy_waypoint(WaypointData * waypoint)
    {
        waypoint->next = nullptr;
        waypoint->previous = nullptr;
        delete waypoint; 
    }

    WaypointStatus CruisingStateManager::append_waypoint(WaypointData* newWaypoint)
    {
        // Gets index of waypoint that comes before appended waypoint
        int previousIndex = 0;
        previousIndex = numWaypoints - 1;

        // Before initializing elements, checks if new waypoint is not a duplicate
        if (previousIndex != -1 && waypointBuffer[previousIndex]->latitude == newWaypoint->latitude && waypointBuffer[previousIndex]->longitude == newWaypoint->longitude) {
            destroy_waypoint(newWaypoint); // To pevent memory leaks from occuring, if there is an error the waypoint is removed from memory.
            return INVALID_PARAMETERS;
        }

        waypointBuffer[numWaypoints] = newWaypoint;

        //If we are initializing the first element
        if (previousIndex == 0) { 
            numWaypoints++;

            return WAYPOINT_SUCCESS;
        }

        // Links previous waypoint with current one
        waypointBuffer[numWaypoints]->previous = waypointBuffer[previousIndex];
        waypointBuffer[previousIndex]->next = waypointBuffer[numWaypoints];

        // Increments array navigation parameters since a new waypoint was added
        numWaypoints++;

        return WAYPOINT_SUCCESS;
    }

    WaypointStatus CruisingStateManager::clear_flight_plan()
    {
        for (int i = 0; i < numWaypoints; ++i)
        {
            if (waypointBuffer[i] != nullptr)
            {
                destroy_waypoint(waypointBuffer[i]);
                waypointBuffer[i] = nullptr;
            }
        }

        // Resets buffer status variables
        numWaypoints = 0;
        currentIndex = 0;

        return WAYPOINT_SUCCESS;
    }

    WaypointStatus CruisingStateManager::get_next_directions(WaypointManager_Data_In currentStatus, WaypointManager_Data_Out *Data)
    {
        // follow waypoints
        float position[3]; 
        // Gets current track
        float currentTrack = (float) currentStatus.track;
        PM::Waypoint::get_coordinates(currentStatus.longitude, currentStatus.latitude, position);
        position[2] = (float) currentStatus.altitude;

        follow_waypoints(waypointBuffer[currentIndex], position, currentTrack);

        // update return data
        update_return_data(Data);
        
        return WAYPOINT_SUCCESS;
    }

    void CruisingStateManager::follow_waypoints(WaypointData* currentWaypoint, float* position, float track)
    {
        if (currentWaypoint->next == nullptr) 
        { // If target waypoint is not defined
            follow_last_line_segment(currentWaypoint, position, track);
        }
        else if (currentWaypoint->next->next == nullptr) 
        { // If waypoint after target waypoint is not defined
            follow_line_segment(currentWaypoint, position, track);
        }
        else
        { // If there are two waypoints after target waypoint
            next_waypoints(currentWaypoint, position, track);
        }
    }

    void CruisingStateManager::next_waypoints(WaypointData* currentWaypoint, float* position, float track)
    {
        float waypointPosition[3]; 
        PM::Waypoint::get_coordinates(currentWaypoint->longitude, currentWaypoint->latitude, waypointPosition);
        waypointPosition[2] = currentWaypoint->altitude;

        // Defines target waypoint
        WaypointData * targetWaypoint = currentWaypoint->next;
        float targetCoordinates[3];
        PM::Waypoint::get_coordinates(targetWaypoint->longitude, targetWaypoint->latitude, targetCoordinates);
        targetCoordinates[2] = targetWaypoint->altitude;

        // Defines waypoint after target waypoint
        WaypointData* waypointAfterTarget = targetWaypoint->next;
        float waypointAfterTargetCoordinates[3];
        PM::Waypoint::get_coordinates(waypointAfterTarget->longitude, waypointAfterTarget->latitude, waypointAfterTargetCoordinates);
        waypointAfterTargetCoordinates[2] = waypointAfterTarget->altitude;

        float waypointDirection[3];
        PM::Waypoint::calculate_direction_to_waypoint(targetCoordinates, waypointPosition, waypointDirection);

        float nextWaypointDirection[3];
        PM::Waypoint::calculate_direction_to_waypoint(waypointAfterTargetCoordinates, targetCoordinates, waypointDirection);


        // // Required turning angle
        // float turningAngle = acos(-DEG_TO_RAD(waypointDirection[0] * nextWaypointDirection[0] + waypointDirection[1] * nextWaypointDirection[1] + waypointDirection[2] * nextWaypointDirection[2]));
        // // Calculates tangent factor that helps determine centre of turn 
        // float tangentFactor = targetWaypoint->turnRadius/tan(turningAngle/2);

        // float halfPlane[3];
        // halfPlane[0] = targetCoordinates[0] - tangentFactor * waypointDirection[0];
        // halfPlane[1] = targetCoordinates[1] - tangentFactor * waypointDirection[1];
        // halfPlane[2] = targetCoordinates[2] - tangentFactor * waypointDirection[2];

        // // Calculates distance to next waypoint
        // float distanceToWaypoint = sqrt(pow(targetCoordinates[0] - position[0],2) + pow(targetCoordinates[1] - position[1],2) + pow(targetCoordinates[2] - position[2],2));
        // distanceToNextWaypoint = distanceToWaypoint; 

        // // Checks if plane is orbiting or flying in a straight line
        // if (orbitPathStatus == PATH_FOLLOW) {
        //     float dotProduct = waypointDirection[0] * (position[0] - halfPlane[0]) + waypointDirection[1] * (position[1] - halfPlane[1]) + waypointDirection[2] * (position[2] - halfPlane[2]);
            
        //     if (dotProduct > 0){
        //         orbitPathStatus = ORBIT_FOLLOW;
        //         if (targetWaypoint->waypointType == HOLD_WAYPOINT) {
        //             inHold = true;
        //             turnDirection = 1; // Automatically turn CCW
        //             turnRadius = targetWaypoint->turnRadius;
        //             turnDesiredAltitude = targetWaypoint->altitude;
        //             turnCenter[0] = targetWaypoint->longitude;
        //             turnCenter[1] = targetWaypoint->latitude;
        //             turnCenter[2] = turnDesiredAltitude;
        //             currentIndex++; 
        //         }
        //     }

        //     follow_straight_path(waypointDirection, targetCoordinates, position, track);
        // } else {
        //     // Determines turn direction (CCW returns 2; CW returns 1)
        //     turnDirection = waypointDirection[0] * nextWaypointDirection[1] - waypointDirection[1] * nextWaypointDirection[0]>0?1:-1;
            
        //     // Since the Earth is not flat *waits for the uproar to die down* we need to do some fancy geometry. Introducing!!!!!!!!!! EUCLIDIAN GEOMETRY! (translation: I have no idea what this line does but it should work)
        //     float euclideanWaypointDirection = sqrt(pow(nextWaypointDirection[0] - waypointDirection[0],2) + pow(nextWaypointDirection[1] - waypointDirection[1],2) + pow(nextWaypointDirection[2] - waypointDirection[2],2)) * ((nextWaypointDirection[0] - waypointDirection[0]) < 0?-1:1) * ((nextWaypointDirection[1] - waypointDirection[1]) < 0?-1:1) * ((nextWaypointDirection[2] - waypointDirection[2]) < 0?-1:1);

        //     // Determines coordinates of the turn center
        //     turnCenter[0] = targetCoordinates[0] + (tangentFactor * (nextWaypointDirection[0] - waypointDirection[0])/euclideanWaypointDirection);
        //     turnCenter[1] = targetCoordinates[1] + (tangentFactor * (nextWaypointDirection[1] - waypointDirection[1])/euclideanWaypointDirection);
        //     turnCenter[2] = targetCoordinates[2] + (tangentFactor * (nextWaypointDirection[2] - waypointDirection[2])/euclideanWaypointDirection);

        //     float dotProduct = nextWaypointDirection[0] * (position[0] - halfPlane[0]) + nextWaypointDirection[1] * (position[1] - halfPlane[1]) + nextWaypointDirection[2] * (position[2] - halfPlane[2]);
            
        //     if (dotProduct > 0){
        //         currentIndex++; 
        //         orbitPathStatus = PATH_FOLLOW;
        //     }

        //     //If two waypoints are parallel to each other (no turns)
        //     if (euclideanWaypointDirection == 0){
        //         // For same reasons above, since the waypoints are parallel, we can switch the current waypoint and target the next one
        //         currentIndex++;

        //         orbitPathStatus = PATH_FOLLOW;
        //     }

        //     outputType = ORBIT_FOLLOW;

        //     follow_orbit(position, track);
        // }

    }

    void CruisingStateManager::follow_line_segment(WaypointData* currentWaypoint, float* position, float track)
    {
        float waypointPosition[3];
        PM::Waypoint::get_coordinates(currentWaypoint->longitude, currentWaypoint->latitude, waypointPosition);
        waypointPosition[2] = currentWaypoint->altitude;

        // Defines target waypoint
        WaypointData * targetWaypoint = currentWaypoint->next;
        float targetCoordinates[3];
        PM::Waypoint::get_coordinates(targetWaypoint->longitude, targetWaypoint->latitude, targetCoordinates);
        targetCoordinates[2] = targetWaypoint->altitude;

        float waypointDirection[3];
        PM::Waypoint::calculate_direction_to_waypoint(targetCoordinates, waypointPosition, waypointDirection);

        float distanceToWaypoint = PM::Waypoint::calculate_distance_to_waypoint(targetCoordinates, waypointPosition);

        int desiredTrack = PM::Waypoint::follow_straight_path(waypointDirection, targetCoordinates, position, track);
        
        outputType = PATH_FOLLOW;
        desiredAltitude = targetCoordinates[2];

        for (int i=0; i<3; ++i)
        {
            direction[i] = waypointDirection[i];
        }


        if (!inHold) {
            turnRadius = 0;
            turnDirection = 0;
        }
    }

    void CruisingStateManager::follow_last_line_segment(WaypointData* currentWaypoint, float* position, float track)
    {
        // Current position is set to waypointPosition
        float waypointPosition[3];
        waypointPosition[0] = position[0];
        waypointPosition[1] = position[1];
        waypointPosition[2] = position[2];

        // Target waypoint is the current waypoint
        WaypointData * targetWaypoint = currentWaypoint;
        float targetCoordinates[3];
        PM::Waypoint::get_coordinates(targetWaypoint->longitude, targetWaypoint->latitude, targetCoordinates);
        targetCoordinates[2] = targetWaypoint->altitude;

        float waypointDirection[3];
        PM::Waypoint::calculate_direction_to_waypoint(targetCoordinates, waypointPosition, waypointDirection);

        float distanceToWaypoint = PM::Waypoint::calculate_distance_to_waypoint(targetCoordinates, waypointPosition);

        float dotProduct = PM::Waypoint::dot_product(waypointDirection, position, targetCoordinates);
        if (dotProduct > 0)
        {
            // set circle parameters over last waypoint
            inHold = true;
            turnDirection = 1; // Automatically turn CCW
            turnRadius = 50;
            turnDesiredAltitude = targetWaypoint->altitude;
            turnCenter[0] = targetWaypoint->longitude;
            turnCenter[1] = targetWaypoint->latitude;
            turnCenter[2] = turnDesiredAltitude; 
        }

        int desiredTrack = PM::Waypoint::follow_straight_path(waypointDirection, targetCoordinates, position, track);
        
        outputType = PATH_FOLLOW;
        desiredAltitude = targetCoordinates[2];

        for (int i=0; i<3; ++i)
        {
            direction[i] = waypointDirection[i];
        }

        if (!inHold) {
            turnRadius = 0;
            turnDirection = 0;
        }
    }

    void CruisingStateManager::update_return_data(WaypointManager_Data_Out *Data) {
        Data->desiredTrack = desiredTrack;
        Data->desiredAltitude =  desiredAltitude;
        Data->distanceToNextWaypoint = distanceToNextWaypoint;
        Data->distanceX = direction[0];
        Data->distanceY = direction[1];
        Data->distanceZ = direction[2];
        Data->rotation = 0;
        // Data->radius = turnRadius;
        // Data->turnDirection = turnDirection;
        Data->errorCode = errorCode;
        Data->isDataNew = dataIsNew;
        dataIsNew = false; 
        Data->timeOfData = 0;
        Data->desiredAirspeed = 0; 
        Data->waypoint_type = outputType;
    }

} // namespace PM