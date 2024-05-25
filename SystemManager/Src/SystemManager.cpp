//
// Created by Gagan Deep Singh on 2023-07-08.
// Edited by Navtaj Hundal on 2023-07-23.
//

#include "SystemManager.hpp"
#include "sbus_receiver.hpp"
#include "sbus_defines.h"
#include "rcreceiver_datatypes.h"
#include "drivers_config.hpp"
#include "independent_watchdog.h"
#include "tim.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include <iostream>

#define TIMEOUT_CYCLES 250000 // 25k = 1 sec fro testing 10/14/2023 => 250k = 10 sec

// TODO: remove this after testing
static BaseType_t taskCreated[3];
static TaskHandle_t taskHandles[3];

static uint32_t DisconnectionCount = 0;
float prevthrottle;
float prevyaw;
float prevroll;
float prevpitch;

SystemManager::SystemManager():
    rcController_(sbus_pointer),
    throttleMotorChannel_(&htim2, TIM_CHANNEL_1),
    yawMotorChannel_(&htim2, TIM_CHANNEL_2),
    rollMotorChannel_(&htim2, TIM_CHANNEL_3),
    pitchMotorChannel_(&htim2, TIM_CHANNEL_4),
    invertedRollMotorChannel_(&htim3, TIM_CHANNEL_1),
    watchdog_(&hiwdg)
{}

SystemManager::~SystemManager() {}

//wrapper functions are needed as FreeRTOS xTaskCreate function does not accept functions that have "this" pointers
void SystemManager::systemCheckTaskWrapper(void *pvParameters) {
    SystemManager *systemManagerInstance = static_cast<SystemManager *>(pvParameters);
    systemManagerInstance->systemCheckTask();
}

void SystemManager::attitudeManagerTaskWrapper(void* pvParameters){
    SystemManager *systemManagerInstance = static_cast<SystemManager *>(pvParameters);  
    systemManagerInstance->attitudeManagerTask();
}

void SystemManager::telemetryManagerTaskWrapper(void* pvParameters){
    SystemManager *systemManagerInstance = static_cast<SystemManager *>(pvParameters);
    systemManagerInstance->telemetryManagerTask();
}

void SystemManager::systemCheckTask(){
    for(;;){
        printf("systemCheckTask memory: %d \r\n", uxTaskGetStackHighWaterMark( NULL ));
        vTaskDelay(1000); 

        /*
        this->rcInputs_ = rcController_->GetRCControl();

        //TO-DO: need to implement it using is_Data_New;
        // boolean is true if data has not changed since the last cycle
        bool is_unchanged{
        	rcInputs_.throttle == prevthrottle &&
        	rcInputs_.yaw == prevyaw &&
			rcInputs_.roll == prevroll &&
			rcInputs_.pitch == prevpitch
        };

        if (is_unchanged) {
        	DisconnectionCount += 1; // if its not changed we increment the timeout counter
        	if (DisconnectionCount > TIMEOUT_CYCLES) { // if timeout has occured
        		DisconnectionCount = TIMEOUT_CYCLES+1; // avoid overflow but keep value above threshold
        		this->rcInputs_.arm = 0; // failsafe
        	}
        } else {
        	DisconnectionCount = 0; //if the data has changed we want to reset out counter
        }
        std::cout<<"system check"<<'\n';
        watchdog_.refreshWatchdog(); // always hit the dog

        //rc inputs need to go to AM
        //AM has buffer
        if(this->rcInputs_.arm >= (SBUS_MAX/2)) {
        		this->throttleMotorChannel_.set(rcInputs_.throttle);
        		this->yawMotorChannel_.set(rcInputs_.yaw);
        		this->rollMotorChannel_.set(SBUS_MAX - rcInputs_.roll);
        		this->pitchMotorChannel_.set(SBUS_MAX - rcInputs_.pitch);
        		this->invertedRollMotorChannel_.set(SBUS_MAX - rcInputs_.roll);

        		prevthrottle = rcInputs_.throttle;
        		prevyaw = rcInputs_.yaw;
        		prevroll = rcInputs_.roll;
        		prevpitch = rcInputs_.pitch;
        }
        else{
            this->throttleMotorChannel_.set(0);
            this->yawMotorChannel_.set(SBUS_MAX/2);
            this->rollMotorChannel_.set(SBUS_MAX/2);
            this->pitchMotorChannel_.set(SBUS_MAX/2);
            this->invertedRollMotorChannel_.set(SBUS_MAX/2);
        }
        */
    }
}

void SystemManager::attitudeManagerTask(){
    for(;;){
        //call AM

        printf("AM memory: %d \r\n", uxTaskGetStackHighWaterMark( NULL ));
        vTaskDelay(2000); 
    }
}

void SystemManager::telemetryManagerTask(){
    for(;;){
        //call TM

        printf("TM memory: %d \r\n", uxTaskGetStackHighWaterMark( NULL ));
        vTaskDelay(3000); 
    }
}

void SystemManager::setup() {
    //BaseType_t xTaskCreate( TaskFunction_t pvTaskCode, const char * const pcName, configSTACK_DEPTH_TYPE usStackDepth, void * pvParameters, UBaseType_t uxPriority, TaskHandle_t * pxCreatedTask ); 
    //                          function's name             description                 size of stack to allocate        parameters for task        priority                    handler 
    taskCreated[0] = xTaskCreate(systemCheckTaskWrapper, "System Check", 150U, this, osPriorityNormal, taskHandles);
    taskCreated[1] = xTaskCreate(attitudeManagerTaskWrapper, "Attitude Manager", 200U, this, osPriorityNormal, taskHandles + 1);
    taskCreated[2] = xTaskCreate(telemetryManagerTaskWrapper, "Telemetry Manager", 200U, this, osPriorityNormal, taskHandles + 2);
}
