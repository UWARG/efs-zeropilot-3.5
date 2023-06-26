#ifndef PM_WAYPOINT_HPP
#define PM_WAYPOINT_HPP

#include <math.h>
#include "MathCOnstants.h"

#include "PM_DataTypes.hpp"

#define MAX_PATH_APPROACH_ANGLE ZP_PI/2
#define EARTH_RADIUS 6378.137

// For calculating desired track. This affects the sensitivity of the given desired tracks
#define K_GAIN(index) (index == 0 ? 0.01 : 1.0f)

namespace PM::Waypoint {

    /**
    * Calculates the direction vector to next waypoint
    *
    * @param[in] float* nextWaypointCoordinates -> next waypoint coordinates
    * @param[in] float* prevWaypointCoordnates -> previous waypoint coordinates
    * @param[out] float* waypointDirection -> waypoint direction vector
    */
    float calculate_direction_to_waypoint(float* nextWaypointCoordinates, float* prevWaypointCoordnates, float* waypointDirection);

    /**
    * Calculate and returns distance to waypoint in metres
    *
    * @param[in] float* nextWaypointCoordinates -> next waypoint coordinates
    * @param[in] float* position -> current position
    */
    float calculate_distance_to_waypoint(float* nextWaypointCoordinates, float* position);

    /**
    * Calculate and return dot product
    *
    * @param[in] float* waypointDirection -> directions to next waypoint
    * @param[in] float* position -> current position
    * @param[in] float* halfPlane -> halfPlane to check orbit, targetCoordinates to check if waypoint is passed
    */
    float dot_product(float* waypointDirection, float* position, float* halfPlane);

    /**
    * Takes GPS long and lat data and converts it into coordinates (better for calculating tracks and stuff)
    *
    * @param[in] long double longitude -> GPS longitide
    * @param[in] long double latitude -> GPS latitude
    * @param[out] float* xyCoordinates -> Array that will store the x and y coordinates of the plane
    */
    void get_coordinates(long double longitude, long double latitude, float* xyCoordinates);

    /**
     * Takes in two points and returns distance in metres
     *
     * @param[in] lat1, long2 --> Point 1
     * @param[in] lat2, long2 --> Point 2
     */
    float get_distance(long double lat1, long double lon1, long double lat2, long double lon2);

    /**
    * Makes the plane follow an orbit with defined radius and direction
    */
    int follow_orbit(float* position, float* turnCenter, float turnDirection, float turnRadius, float track);
    
    /**
    * Makes a plane follow a straight path (straight line following)
    */
    int follow_straight_path(float* waypointDirection, float* targetWaypoint, float* position, float track);

} // namespace PM::WaypointManager

#endif
