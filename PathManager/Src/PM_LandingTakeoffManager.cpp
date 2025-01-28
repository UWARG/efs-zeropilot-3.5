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

/*
struct AttitudeManagerInput {
    float dist_forward; 
    float dist_right; 
    float dist_up; 
    float magnitude = 0;
    float heading = 0;
    double speed = 0;
}; 
*/

AM::AttitudeManagerInput LandingTakeoffManager::createTakeoffWaypointFixedWing(const LOS::LosSFData & input)
{
    AM::AttitudeManagerInput desiredWaypoint;

    double currentSpeed = input.airspeed;

    // rolling stage
    if (currentSpeed < TAKEOFF_TARGET_SPEED_FIXED_WING) {
        desiredWaypoint.dist_forward = 1;
        desiredWaypoint.dist_right = 0;
        desiredWaypoint.dist_up = 0;

        desiredWaypoint.speed = TAKEOFF_TARGET_SPEED_FIXED_WING;
        desiredWaypoint.magnitude = 0; // Use velocity controller
    }
    // climbing stage
    else if(currentSpeed >= TAKEOFF_TARGET_SPEED_FIXED_WING){
        desiredWaypoint.dist_forward = 1; 
        desiredWaypoint.dist_right = 0;
        desiredWaypoint.dist_up = 1;

        desiredWaypoint.speed = input.rateOfClimb; // TODO: Figure out if this is correct
        desiredWaypoint.magnitude = 0; // Use velocity controller
    }

    return desiredWaypoint;
}

AM::AttitudeManagerInput LandingTakeoffManager::createLandingWaypointFixedWing(const LOS::LosSFData & input){
    // AM::AttitudeManagerInput* desiredWaypoint = new AM::AttitudeManagerInput[3];
    AM::AttitudeManagerInput desiredWaypoint;
    double currentSpeed = input.airspeed;
    double currentAltitude = input.altitude;
    double flareHeight = 5; // TODO: Figure out what this value should be

    // Slope Stage
    if (currentAltitude > flareHeight) {
        desiredWaypoint.dist_forward = 1;
        desiredWaypoint.dist_right = 0;
        desiredWaypoint.dist_up = -1;

        desiredWaypoint.speed = currentSpeed * 0.95; // TODO: Figure out if this is correct
        desiredWaypoint.magnitude = 0; // Use velocity controller
    }
    // Flare Stage
    else if(currentAltitude <= flareHeight && currentAltitude > 0) {
        desiredWaypoint.dist_forward = 1; 
        desiredWaypoint.dist_right = 0;
        desiredWaypoint.dist_up = -1;

        desiredWaypoint.speed = STALL_SPEED_FIXED_WING * 1.1; // TODO: Figure out if this is correct
        desiredWaypoint.magnitude = 0; // Use velocity controller
    } 
    // Landing Stage
    else if(currentAltitude <= 0) {
        desiredWaypoint.dist_forward = 0; 
        desiredWaypoint.dist_right = 0;
        desiredWaypoint.dist_up = 0;

        desiredWaypoint.speed = 0; // TODO: Figure out if this is correct
        desiredWaypoint.magnitude = 0; // Use velocity controller
    }

    return desiredWaypoint;
}