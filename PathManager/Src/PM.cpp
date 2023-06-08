#include "PM.hpp"
#include "PM_States.hpp"

namespace PM {

PathManager::PathManager()
{
    currentState = &FlightModeSelector::getInstance();
    status = IN_CYCLE;
    flight_stage = DISARMED;
    isError = false;
}

void PathManager::setState(PathManagerState& newState)
{
    currentState->exit(this);
    currentState = &newState;

    if (*currentState == CommsWithAttitude::getInstance()) {
        status = COMPLETED_CYCLE;
    } else if (*currentState == FatalFailureMode::getInstance()) {
        status = FAILURE_MODE;
    } else {
        status = IN_CYCLE;
    }

    currentState->enter(this);
}

void PathManager::execute()
{
    currentState->execute(this);
}

/* Getters and Setters for Queue Ids */
void PathManager::storeSmPmQueue(osMessageQId queueId) {
    SM_to_PM_queue = queueId;
}

osMessageQId PathManager::getSmPmQueue() {
    return SM_to_PM_queue;
}

void PathManager::storePmAmQueue(osMessageQId queueId) {
    PM_to_AM_queue = queueId;
}

osMessageQId PathManager::getPmAmQueue() {
    return PM_to_AM_queue;
}

/* Getters and Setters for SM and AM Structs */
void PathManager::setSmStruct(const SM_PM_Commands &from_sm_data) {
    sm_instructions = from_sm_data;
}

SM_PM_Commands PathManager::getSmStruct(){
    return sm_instructions; 
}

void PathManager::setAmStruct(const AM::AttitudeManagerInput &am_instructions){
    am_data = am_instructions; 
}

AM::AttitudeManagerInput PathManager::getAmStruct() {
    return am_data; 

}


/*void PathManager::storeUsePmFlag() {
    
}*/

}
