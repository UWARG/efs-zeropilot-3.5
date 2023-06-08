
/* Related Confluence Pages
    https://uwarg-docs.atlassian.net/wiki/spaces/ZP/pages/1607630921/Landing+and+Takeoff
*/

#ifndef ZPSW3_LANDING_TAKEOFF_MANAGER_HPP
#define ZPSW3_LANDING_TAKEOFF_MANAGER_HPP
#include "CommonDataTypes.hpp"
#include "PM_DataTypes.hpp"

/********************************************************************
* Constants
********************************************************************/

const double TAKEOFF_TARGET = 4.0;
const double LANDING_SPEED = 0.2;
const double MAX_SPEED = 1.0;

class LandingTakeoffManager{

    public:
        /******************
         * EXTERNAL FUNCTIONS
         ******************/

        /*
            This function sets the ground height if it is called for the first time
            This function returns the takeoff altitude target (ground height + TAKEOFF_TARGET)

            @param double currentAltitude - this variable holds the current altitude value of the aircraft

            @return double - this structure holds the altitude point
        */
        double getTakeoffAltitudeTarget(double currentAltitude);

        /*
            This function returns the ground height that was set during takeoff

            @param double currentAltitude - this variable holds the current altitude value of the aircraft

            @return double - this structure holds the altitude point
        */
        double getLandingAltitudeTarget(double currentAltitude);

        /******************
         * LANDING FUNCTIONS
         ******************/

        /*
            This function returns a waypoint that the drone should follow
            This waypoint does not have a horizontal component yet, this will come as corrections from the CV team

            @param const SFOutput_t & input - this variable holds the sensor fusion module output

            @return WaypointData - this structure holds the climb point, which the plane will follow until it exits below a certain altitude
        */
        AM::AttitudeManagerInput createLandingWaypoint(const LOS::LosSFData & input);

        /******************
         * TAKEOFF FUNCTIONS
         *****************/
        /*

        /*
            This function returns a waypoint that the drone should follow
            This waypoint does not have a horizontal component yet, this will come as corrections from the CV team

            @param const SFOutput_t & input - this variable holds the sensor fusion module output

            @return WaypointData - this structure holds the climb point, which the plane will follow until it exits at a certain altitude
        */
        AM::AttitudeManagerInput createTakeoffWaypoint(const LOS::LosSFData & input);

        /***********************************
        COMMON LANDING AND TAKEOFF FUNCTIONS
        ************************************/

        /*
            This function returns the desired climb/descend speed given the current altitude
            Because the takeoff and landing target velocities 

            @param double currentAltitude - this double holds the current aircraft altitude

            @param double rangeConstant - this double holds the range constant for the path

            @return double - this function will return the desired climb speed for the aircraft
        */
        double getSpeedTarget(double currentAltitude, double rangeConstant);

        /*
            This function returns a waypoint that the drone should follow
            This waypoint does not have a horizontal component yet, this will come as corrections from the CV team

            @param double midpointAltitude - this variable holds the midpoint of the altitude range.

            @return double - this structure holds the range constant to determine the velocity based on altitude.
        */
        double getRangeConstant(double midpointAltitude);

    private:
        double groundHeight = -1; // Set by takeoff and used by landing
        double takeoffRangeConstant = 0; // Value set based on the drone altitude when transitioning to takeoff
        double landingRangeConstant = 0; // Value set based on the drone altitude when transitioning to landing
        // TODO: Reset these constants to 0 in case landing is transitioned into multiple times in a flight
        // Note: a range constant value of 0 means a velocity of 0, so drone only moves once value set.
};

#endif
