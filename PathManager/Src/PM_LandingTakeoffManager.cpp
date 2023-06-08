#include "PM_LandingTakeoffManager.hpp"
#include <stdio.h>      /* printf */
#include <math.h>
//#include "MathConstants.hpp"

double LandingTakeoffManager::getTakeoffAltitudeTarget(double currentAltitude)
{
    // Store the ground height when first planning the path
    if (groundHeight < 0) {
        groundHeight = currentAltitude;
    }

    //Calculate the takeoff range constant if just entering takeoff state.
    if (takeoffRangeConstant == 0) {
        takeoffRangeConstant = getRangeConstant(groundHeight + 2);
    }

    // Return the takeoff altitude target for use when determining when to exit Takeoff
    return groundHeight + TAKEOFF_TARGET;
}

double LandingTakeoffManager::getLandingAltitudeTarget(double currentAltitude)
{
    //Calculate the landing range constant if just entering landing state.
    if (landingRangeConstant == 0) {
        landingRangeConstant = getRangeConstant(groundHeight + (currentAltitude - groundHeight) / 2);
    }
    // Return ground height for use to determine when to transition out of Landing
    return groundHeight;
}

AM::AttitudeManagerInput LandingTakeoffManager::createTakeoffWaypoint(const LOS::LosSFData & input)
{
    AM::AttitudeManagerInput desiredWaypoint;

    //TODO: update AM::AttitudeManagerInput to have these member names 
    desiredWaypoint.dist_forward = 0; // No Horizontal movement for MVP
    desiredWaypoint.dist_right = 0; // No Horizontal movement for MVP
    desiredWaypoint.dist_up = 1;
   // desiredWaypoint.waypoint_type = TAKEOFF_WAYPOINT;

    double currentAltitude = input.altitude;

    desiredWaypoint.speed = getSpeedTarget(currentAltitude, takeoffRangeConstant);
    desiredWaypoint.magnitude = 0; // Use velocity controller

    return desiredWaypoint;
}

AM::AttitudeManagerInput LandingTakeoffManager::createLandingWaypoint(const LOS::LosSFData & input)
{
    AM::AttitudeManagerInput desiredWaypoint;

    desiredWaypoint.dist_forward = 0; // No Horizontal movement for MVP
    desiredWaypoint.dist_right = 0; // No Horizontal movement for MVP
    desiredWaypoint.dist_up = -1;
    //desiredWaypoint.waypoint_type = LANDING_WAYPOINT;

    double currentAltitude = input.altitude;

    desiredWaypoint.speed = getSpeedTarget(currentAltitude, landingRangeConstant);
    desiredWaypoint.magnitude = 0; // Use velocity controller

    return desiredWaypoint;
}

double LandingTakeoffManager::getSpeedTarget(double currentAltitude, double rangeConstant)
{
    double speedTarget;
    speedTarget = MAX_SPEED * exp(-1.0 * (pow((currentAltitude - groundHeight - (TAKEOFF_TARGET / 2)), 2) / rangeConstant)); 
    return speedTarget;
}

double LandingTakeoffManager::getRangeConstant(double midpointAltitude)
{ 
    return sqrt((pow(midpointAltitude, 2)) / (-2.0 * (log(LANDING_SPEED / MAX_SPEED))));
    // Note log here is log base e (ln)
}
