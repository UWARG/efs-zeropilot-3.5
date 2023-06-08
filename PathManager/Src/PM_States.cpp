#include "PM_States.hpp"

/***********************************************************************************************************************
 * Static Member Variable Declarations
 **********************************************************************************************************************/

namespace PM {
SM_PM_Commands CommsWithSystemManager::incomingData;
//SFOutput_t SensorFusion::sfOutputData;
//IMU_Data_t SensorFusion::imudata;
AM::AttitudeManagerInput CommsWithAttitude::receivedData;


// TAKEOFF STAGE VARIABLES 
WaypointData * TakeoffStage::currentLocation;    
WaypointData * TakeoffStage::targetWaypoint;
LOS::LosSFData TakeoffStage::LOSData; 
AM::AttitudeManagerInput TakeoffStage::takeoffDataForAM; 

// LANDING STAGE VARIABLES 
WaypointData * LandingStage::currentLocation;
WaypointData * LandingStage::targetWaypoint;
LOS::LosSFData LandingStage::LOSData; 
AM::AttitudeManagerInput LandingStage::landingDataForAM;


constexpr int LANDING_TIME_THRESHOLD {5};


/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/


PathManagerState& CommsWithSystemManager::getInstance()
{
    static CommsWithSystemManager singleton;
    return singleton;
}

#ifdef UNIT_TESTING

#include "cruisingMode.hpp"

static Telemetry_Waypoint_Data_t createTelemetryWaypoint(long double lon, long double lat, int alt, float turnRadius, int type) {
    Telemetry_Waypoint_Data_t waypoint {};

    waypoint.longitude = lon;
    waypoint.latitude = lat;
    waypoint.altitude = alt;
    waypoint.turnRadius = turnRadius;
    waypoint.waypointType = type;

    return waypoint;
}

#endif

void FlightModeSelector::execute(PathManager* pathMgr){
    if(pathMgr->isError)
    {
        pathMgr->setState(FatalFailureMode::getInstance());
    }


  

    // Check if altitude target has been reached for takeoff to start cruising stage. 
    if(pathMgr->getSmStruct().sf_data.altitude > pathMgr->vtol_manager.getTakeoffAltitudeTarget(pathMgr->getSmStruct().sf_data.altitude) 
        && pathMgr->flight_stage == TAKEOFF)
    {
        pathMgr->flight_stage = CRUISING;

    }

     if(pathMgr->getSmStruct().landing_initiation.start_landing)
    {
        pathMgr->flight_stage = LANDING;
    
    }
 
    switch(pathMgr->flight_stage){
        case LANDING:
            pathMgr->setState(LandingStage::getInstance());
            break;
        case CRUISING:
            pathMgr->setState(CruisingStage::getInstance());
            break;
        case TAKEOFF:
            pathMgr->setState(TakeoffStage::getInstance());
            break;
        default:
            // Any other stage (Disarmed, Landed) should go back to comms with AM
            pathMgr->setState(CommsWithAttitude::getInstance()); 
    }
}

PathManagerState& FlightModeSelector::getInstance()
{
    static FlightModeSelector singleton; 
    return singleton; 
}

void CruisingStage::execute(PathManager* pathMgr)
{
    /* TODO: All of this 

    // telementryData is where we want to go, target waypoint
    fijo * telemetryData = CommsWithTelemetry::GetTelemetryIncomingData(); // Get struct from telemetry state with all of the commands and values.
    SFOutput_t * sensFusionOutput = SensorFusion::GetSFOutput(); // Get sensor fusion data

    // Set waypoint manager input struct 
    // input data is the current position, should be middle waypoint
    _inputdata.track = sensFusionOutput->track; // Gets track
    _inputdata.longitude = sensFusionOutput->longitude; // setting current waypoint longitude to SF longitude 
    _inputdata.latitude = sensFusionOutput->latitude;
    _inputdata.altitude = sensFusionOutput->altitude;

    //initializing current and target waypoint, setting the relationship of their pointers 
    ModifyFlightPathErrorCode editError = editFlightPath(cruisingStateManager, telemetryData, this->_inputdata); // Edit flight path if applicable
    
    GetNextDirectionsErrorCode pathError = pathFollow(cruisingStateManager, telemetryData, _inputdata, &_outputdata); // Get next direction or modify flight behaviour pattern
    setReturnValues(&_returnToGround, cruisingStateManager, editError, pathError); // Set error codes

    */

    if(pathMgr->isError)
    {
        pathMgr->setState(FatalFailureMode::getInstance());
    }
    else
    {
         pathMgr->setState(CommsWithAttitude::getInstance()); 
    }
    
}

PathManagerState& CruisingStage::getInstance()
{
    static CruisingStage singleton;
    return singleton;
}

void LandingStage::execute(PathManager* pathMgr)
{
    pathMgr->flight_stage = LANDING;

    //load in sensor fusion data and telemtry data into input structure
    LOSData = pathMgr->getSmStruct().sf_data; 


   // Creating AM struct to send takeoff data using current SF data.
    landingDataForAM = pathMgr->vtol_manager.createLandingWaypoint(LOSData);    

   /* waypointInput.latitude = input.sensorOutput->latitude;
    waypointInput.longitude = input.sensorOutput->longitude;
    waypointInput.altitude = input.sensorOutput->altitude;
    waypointInput.track = input.sensorOutput->track;*/

    // if(LandingStage::waypointStatus != WAYPOINT_SUCCESS)
    // {
    //     pathMgr->isError = true;
    // }

    if(pathMgr->isError)
    {
        pathMgr->setState(FatalFailureMode::getInstance());
    }
    else
    {
        pathMgr->setState(CommsWithAttitude::getInstance());
    }
}

PathManagerState& LandingStage::getInstance()
{
    static LandingStage singleton;
    return singleton;
}


/****************************************************************************************************
TAKEOFF STATE FUNCTIONS
****************************************************************************************************/

void TakeoffStage::execute(PathManager* pathMgr)
{
    //load in sensor fusion data and telemtry data into input structure
    LOSData = pathMgr->getSmStruct().sf_data; 

       /** waypointInput.latitude = input.sensorOutput->latitude;
        waypointInput.longitude = input.sensorOutput->longitude;
        waypointInput.altitude = input.sensorOutput->altitude;
        //waypointInput.track = input.sensorOutput->track;*/ 

    // Creating AM struct to send takeoff data using current SF data.
    takeoffDataForAM = pathMgr->vtol_manager.createTakeoffWaypoint(LOSData);    
        

    // if(waypointStatus != WAYPOINT_SUCCESS)
    // {
    //     pathMgr->isError = true;
    // }

    if(pathMgr->isError)
    {
        pathMgr->setState(FatalFailureMode::getInstance());
    }
    else
    {
        pathMgr->setState(CommsWithAttitude::getInstance());
    }
}

PathManagerState& TakeoffStage::getInstance()
{
    static TakeoffStage singleton;
    return singleton;
}

void CommsWithAttitude::execute(PathManager* pathMgr)
{

    /*
    for after Dec 12
    _WaypointManager_Data_Out * waypointOutput {}; 
    
    //deciding which stage we get output data from 
    switch(pathMgr->flight_stage){
        case LANDING:
            waypointOutput = LandingStage::GetOutputData();
            break;
        case CRUISING:
            waypointOutput = cruisingState::GetOutputData();
            break;
        case PREFLIGHT:
            waypointOutput = PreflightStage::GetOutputData();
            break;
        case TAKEOFF:
           waypointOutput = TakeoffStage::GetOutputData();
            break;
        default:
            waypointOutput = cruisingState::GetOutputData();
    }
    */
    
    AM::AttitudeManagerInput* outputData {};
    switch(pathMgr->flight_stage){
            case LANDING:
                outputData = LandingStage::getLandingDataForAM();
                break;
            case CRUISING:
               // waypointOutput = cruisingState::GetOutputData();
                break;
            case TAKEOFF:
                outputData = TakeoffStage::getTakeoffDataForAM(); 
                break; 
            default:
                break; 
        }
    pathMgr->setAmStruct(*outputData); 

    
     if(pathMgr->isError)
    {
        pathMgr->setState(FatalFailureMode::getInstance());
    }
}

PathManagerState& CommsWithAttitude::getInstance()
{
    static CommsWithAttitude singleton;
    return singleton;
}

void FatalFailureMode::execute(PathManager* pathMgr)
{
    pathMgr->setState(FatalFailureMode::getInstance());
}

PathManagerState& FatalFailureMode::getInstance()
{
    static FatalFailureMode singleton;
    return singleton;
}

}

