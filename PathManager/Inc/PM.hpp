#ifndef ZPSW3_PM_HPP
#define ZPSW3_PM_HPP

#include <stdint.h>
#include "cmsis_os.h"

#include "PM_StateManager.hpp"
#include "CommonDataTypes.hpp"
#include "PM_DataTypes.hpp"
#include "PM_LandingTakeoffManager.hpp"

namespace PM
{
    class PathManagerState;

    // Gives status of Path Manager so we know when it has completed a cycle, or entered failure mode.
    enum Path_Manager_Cycle_Status {COMPLETED_CYCLE = 0, IN_CYCLE, FAILURE_MODE};

    class PathManager
    {
        public:
            PathManager();
            inline PathManagerState* getCurrentState() const {return currentState;}
            void execute();
            void setState(PathManagerState& newState);
            Path_Manager_Cycle_Status getStatus() {return status;}

            //used to determine the stage of the landing sequence
            LandingTakeoffManager vtol_manager;
            FlightStage flight_stage;
            bool isError;

            void storeSmPmQueue(osMessageQId queueId);
            osMessageQId getSmPmQueue();

            void storePmAmQueue(osMessageQId queueId);
            osMessageQId getPmAmQueue();

        // void storeUsePmFlag();

            //GlobalFlag getUsePmFlag(); 

            void setSmStruct(const SM_PM_Commands &from_sm_data); 
            SM_PM_Commands getSmStruct(); 

            AM::AttitudeManagerInput getAmStruct(); 
            void setAmStruct(const AM::AttitudeManagerInput &am_instructions); 




        
        private:
            PathManagerState* currentState;
            Path_Manager_Cycle_Status status;

            // Message Q
            osMessageQId SM_to_PM_queue;
            osMessageQId PM_to_AM_queue;

            SM_PM_Commands sm_instructions; 
            AM::AttitudeManagerInput am_data; 
    };

}



#endif

