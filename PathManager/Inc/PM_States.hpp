#ifndef ZPSW3_PM_STATES_HPP
#define ZPSW_PM_STATES_HPP

#include "PM_StateManager.hpp"
#include "PM.hpp"
#include "PM_LandingTakeoffManager.hpp"
#include "CommonDataTypes.hpp"
#include "PM_DataTypes.hpp"

namespace PM {

class CommsWithAttitude : public PathManagerState
{
    public:
        void enter(PathManager* pathMgr) {(void) pathMgr;}
        void execute(PathManager* pathMgr);
        void exit(PathManager* pathMgr) {(void) pathMgr;}
        static PathManagerState& getInstance();
        static AM::AttitudeManagerInput* GetCommFromAttitude(void) { return &receivedData; }
    private:
        CommsWithAttitude() {  } // Initializes module
        CommsWithAttitude(const CommsWithAttitude& other);
        CommsWithAttitude& operator =(const CommsWithAttitude& other);
        static AM::AttitudeManagerInput receivedData;
};

class CommsWithSystemManager : public PathManagerState
{
    public:
        void enter(PathManager* pathMgr) {(void) pathMgr;}
        void execute(PathManager* pathMgr);
        void exit(PathManager* pathMgr) {(void) pathMgr;}
        static PathManagerState& getInstance();
        static SM_PM_Commands* GetSMIncomingData(void) {return &incomingData;}
        //void setSMIncomingData()
    private:
        CommsWithSystemManager() {  }
        CommsWithSystemManager(const CommsWithSystemManager& other);
        CommsWithSystemManager& operator =(const CommsWithSystemManager& other);
        static SM_PM_Commands incomingData; // Stores the commands sent by telemetry for easy access by other states in the Pathmanager
};


class FlightModeSelector : public PathManagerState
{
    public:
        void enter(PathManager* pathMgr) {(void) pathMgr;}
        void execute(PathManager* pathMgr);
        void exit(PathManager* pathMgr) {(void) pathMgr;}
        static PathManagerState& getInstance();
    private:
        FlightModeSelector() {}
        FlightModeSelector(const FlightModeSelector& other);
        FlightModeSelector& operator =(const FlightModeSelector& other);

};

class TakeoffStage : public PathManagerState
{
    public:
        void enter(PathManager* pathMgr) {(void) pathMgr;}
        void execute(PathManager* pathMgr);
        void exit(PathManager* pathMgr) {(void) pathMgr;}
        static PathManagerState& getInstance();
        static AM::AttitudeManagerInput* getTakeoffDataForAM() {return &takeoffDataForAM;} 
    private:
        static WaypointData * currentLocation;
        static WaypointData * targetWaypoint;
        //static WaypointManager_Data_In waypointInput;
        //static WaypointManager_Data_Out waypointOutput;
        static LOS::LosSFData LOSData; 
        static AM::AttitudeManagerInput takeoffDataForAM; 
        TakeoffStage() {}
        TakeoffStage(const TakeoffStage& other);
        TakeoffStage& operator =(const TakeoffStage& other);
};


class CruisingStage : public PathManagerState
{
    public:
        void enter(PathManager* pathMgr) {(void) pathMgr;}
        void execute(PathManager* pathMgr);
        void exit(PathManager* pathMgr) {(void) pathMgr;}
        static PathManagerState& getInstance();
        static WaypointManager_Data_Out* GetOutputData(void) {return &_outputdata;}
       // static _CruisingState_Telemetry_Return* GetErrorCodes(void) {return &_returnToGround;}

        #ifdef UNIT_TESTING
            WaypointManager* GetWaypointManager(void) {return &cruisingStateManager;}
            int* GetWaypointIdArray(void) {return waypointIDArray;}
        #endif
    private:
        CruisingStage() {}
        CruisingStage(const CruisingStage& other);
        CruisingStage& operator =(const CruisingStage& other);

        //WaypointManager cruisingStateManager;
        static WaypointManager_Data_In _inputdata;
        static WaypointManager_Data_Out _outputdata;
       // static _CruisingState_Telemetry_Return _returnToGround;
     
}; 

class LandingStage : public PathManagerState
{
    public:
        void enter(PathManager* pathMgr) {(void) pathMgr;}
        void execute(PathManager* pathMgr);
        void exit(PathManager* pathMgr) {(void) pathMgr;}
        static PathManagerState& getInstance();
        static AM::AttitudeManagerInput* getLandingDataForAM() {return &landingDataForAM;} 
    private:
        //static WaypointManager_Data_In waypointInput;
        //static WaypointManager_Data_Out waypointOutput;
        static WaypointData * currentLocation;
        static WaypointData * targetWaypoint;
        static LOS::LosSFData LOSData; 
        static AM::AttitudeManagerInput landingDataForAM;
        LandingStage() {}
        LandingStage(const LandingStage& other);
        LandingStage& operator =(const LandingStage& other);

};




class FatalFailureMode : public PathManagerState
{
    public:
        void enter(PathManager* pathMgr) {(void) pathMgr;}
        void execute(PathManager* pathMgr);
        void exit(PathManager* pathMgr) {(void) pathMgr;}
        static PathManagerState& getInstance();
    private:
        FatalFailureMode() {}
        FatalFailureMode(const FatalFailureMode& other);
        FatalFailureMode& operator =(const FatalFailureMode& other);
};
}
#endif

