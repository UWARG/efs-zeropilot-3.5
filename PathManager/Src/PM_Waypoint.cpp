#include "PM_Waypoint.hpp"

namespace PM::Waypoint {

    float calculate_direction_to_waypoint(float* nextWaypointCoordinates, float* prevWaypointCoordnates, float* waypointDirection)
    {
        float norm = sqrt(pow(nextWaypointCoordinates[0] - prevWaypointCoordnates[0],2) + pow(nextWaypointCoordinates[1] - prevWaypointCoordnates[1],2) + pow(nextWaypointCoordinates[2] - prevWaypointCoordnates[2],2));
        waypointDirection[0] = (nextWaypointCoordinates[0] - prevWaypointCoordnates[0])/norm;
        waypointDirection[1] = (nextWaypointCoordinates[1] - prevWaypointCoordnates[1])/norm;
        waypointDirection[2] = (nextWaypointCoordinates[2] - prevWaypointCoordnates[2])/norm;
    }

    float calculate_distance_to_waypoint(float* nextWaypointCoordinates, float* position)
    {
        return sqrt(pow(nextWaypointCoordinates[0] - position[0],2) + pow(nextWaypointCoordinates[1] - position[1],2) + pow(nextWaypointCoordinates[2] - position[2],2));
    }

    float dot_product(float* waypointDirection, float* position, float* halfPlane)
    {
        return waypointDirection[0] * (position[0] - halfPlane[0]) + waypointDirection[1] * (position[1] - halfPlane[1]) + waypointDirection[2] * (position[2] - halfPlane[2]);
    }
    
    void get_coordinates(long double longitude, long double latitude, float* xyCoordinates, float relativeLatitude, float relativeLongitude) { // Parameters expected to be in degrees
        xyCoordinates[0] = get_distance(relativeLatitude, relativeLongitude, relativeLatitude, longitude); //Calculates longitude (x coordinate) relative to defined origin (RELATIVE_LONGITUDE, RELATIVE_LATITUDE)
        xyCoordinates[1] = get_distance(relativeLatitude, relativeLongitude, latitude, relativeLongitude); //Calculates latitude (y coordinate) relative to defined origin (RELATIVE_LONGITUDE, RELATIVE_LATITUDE)
    }

    float get_distance(long double lat1, long double lon1, long double lat2, long double lon2) { // Parameters expected to be in degrees
        // Longitude and latitude stored in degrees
        // This calculation uses the Haversine formula
        long double change_in_Lat = DEG_TO_RAD(lat2 - lat1); //Converts change in latitude to radians
        long double change_in_lon = DEG_TO_RAD(lon2 - lon1); //Converts change in longitude to radians

        double haversine_ans = sin(change_in_Lat / 2) * sin(change_in_Lat / 2) + cos(DEG_TO_RAD(lat1)) * cos(DEG_TO_RAD(lat2)) * sin(change_in_lon / 2) * sin(change_in_lon / 2); // In kilometers

        if ((change_in_Lat >= 0 && change_in_lon >=0)||(change_in_Lat < 0 && change_in_lon < 0)){
            return EARTH_RADIUS * (2 * atan2(sqrt(haversine_ans),sqrt(1 - haversine_ans))) * 1000; //Multiply by 1000 to convert to metres
        } else {
            return EARTH_RADIUS * (2 * atan2(sqrt(haversine_ans),sqrt(1 - haversine_ans))) * -1000;
        }
    }

    int follow_orbit(float* position, float* turnCenter, float turnDirection, float turnRadius, float track) {
        track = DEG_TO_RAD(90 - track);

        // Distance from centre of circle
        float orbitDistance = sqrt(pow(position[0] - turnCenter[0],2) + pow(position[1] - turnCenter[1],2));
        float courseAngle = atan2(position[1] - turnCenter[1], position[0] - turnCenter[0]); // (y,x) format

        // Normalizes angles
        // First gets the angle between 0 and 2 pi
        if (courseAngle - track >= 2 * ZP_PI) {
            courseAngle = fmod(courseAngle, 2 * ZP_PI);
        } else if (courseAngle - track < 0.0) {
            courseAngle = fmod(courseAngle, 2 * ZP_PI) + 2 * ZP_PI;
        }
        // Now ensures that courseAngle is between -pi and pi
        if (courseAngle > ZP_PI && courseAngle <= 2 * ZP_PI) {
            courseAngle -= 2 * ZP_PI;
        }

        // Desired track
        int calcTrack = round(90 - RAD_TO_DEG(courseAngle + turnDirection * (ZP_PI/2 + atan(K_GAIN(ORBIT_FOLLOW) * (orbitDistance - turnRadius)/turnRadius)))); //Track in degrees 
        
        // Normalizes track (keeps it between 0.0 and 259.9999)
        if (calcTrack >= 360.0) {
            calcTrack = fmod(calcTrack, 360.0);
        } else if (calcTrack < 0.0) {
            calcTrack = fmod(calcTrack, 360.0) + 360.0;
        }
        
        return calcTrack;
    }

    int follow_straight_path(float* waypointDirection, float* targetWaypoint, float* position, float track) {
        track = DEG_TO_RAD(90 - track);//90 - track = track to cartesian track
        float courseAngle = atan2(waypointDirection[1], waypointDirection[0]); // (y,x) format
        
        // Normalizes angles
        // First gets the angle between 0 and 2 pi
        if (courseAngle - track >= 2 * ZP_PI) {
            courseAngle = fmod(courseAngle, 2 * ZP_PI);
        } else if (courseAngle - track < 0.0) {
            courseAngle = fmod(courseAngle, 2 * ZP_PI) + 2 * ZP_PI;
        }
        // Now ensures that courseAngle is between -pi and pi
        if (courseAngle > ZP_PI && courseAngle <= 2 * ZP_PI) {
            courseAngle -= 2 * ZP_PI;
        }

        // Calculates desired track
        float pathError = -sin(courseAngle) * (position[0] - targetWaypoint[0]) + cos(courseAngle) * (position[1] - targetWaypoint[1]);
        int calcTrack = 90 - RAD_TO_DEG(courseAngle - MAX_PATH_APPROACH_ANGLE * 2/ZP_PI * atan(K_GAIN(PATH_FOLLOW) * pathError)); //Heading in degrees (magnetic) 
        
        // Normalizes track (keeps it between 0.0 and 259.9999)
        if (calcTrack >= 360.0) {
            calcTrack = fmod(calcTrack, 360.0);
        } else if (calcTrack < 0.0) {
            calcTrack = fmod(calcTrack, 360.0) + 360.0;
        }
        
        return calcTrack;
    }

} // namespace PM::WaypointManager

