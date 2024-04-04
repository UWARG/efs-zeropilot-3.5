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

void SystemManager::systemCheckTask(void *pvParameters){
    uint8_t msg[] = "systemCheckTask running (every second)...\n";
    for(;;){
        HAL_UART_Transmit(&huart1, msg, sizeof(msg), 100);
        vTaskDelay(pdMS_TO_TICKS(1000)); 



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
    }
}

//wrapper functions are needed as FreeRTOS xTaskCreate function does not accept functions that have "this" pointers
void SystemManager::systemCheckTaskWrapper(void *pvParameters) {

    SystemManager *systemManagerInstance = static_cast<SystemManager *>(pvParameters);
    systemManagerInstance->systemCheckTask(pvParameters);
}

void SystemManager::attitudeManagerTask(void *pvParameters){
    uint8_t msg[] = "AM running (every 2 seconds)...\n";
    for(;;){
        //call AM
        HAL_UART_Transmit(&huart1, msg, sizeof(msg), 100);
        vTaskDelay(pdMS_TO_TICKS(2000)); 

        watchdog_.refreshWatchdog(); // always hit the dog
    }
}

void SystemManager::attitudeManagerTaskWrapper(void* pvParameters){
    SystemManager *systemManagerInstance = static_cast<SystemManager *>(pvParameters);
    systemManagerInstance->attitudeManagerTask(pvParameters);
}

void SystemManager::telemetryManagerTask(void *pvParameters){
    uint8_t msg[] = "TM running (every 3 seconds)...\n";
    for(;;){
        //call TM
        HAL_UART_Transmit(&huart1, msg, sizeof(msg), 100);
        vTaskDelay(pdMS_TO_TICKS(3000)); 
        
        watchdog_.refreshWatchdog(); // always hit the dog
    }
}

void SystemManager::telemetryManagerTaskWrapper(void* pvParameters){
    SystemManager *systemManagerInstance = static_cast<SystemManager *>(pvParameters);
    systemManagerInstance->attitudeManagerTask(pvParameters);
}


void SystemManager::setup() {
    TaskHandle_t hSystemCheck = NULL;

    //BaseType_t xTaskCreate( TaskFunction_t pvTaskCode, const char * const pcName, configSTACK_DEPTH_TYPE usStackDepth, void * pvParameters, UBaseType_t uxPriority, TaskHandle_t * pxCreatedTask ); 
    //                          function's name             description                 size of stack to allocate        parameters for task        priority                    handler
    xTaskCreate(systemCheckTaskWrapper, "System Check", 500U, NULL, osPriorityNormal, &hSystemCheck);

    TaskHandle_t hAM = NULL;
    xTaskCreate(attitudeManagerTaskWrapper, "Attitude Manager", 500U, NULL, osPriorityNormal, &hAM);

    TaskHandle_t hTM = NULL;
    xTaskCreate(telemetryManagerTaskWrapper, "Telemetry Manager", 500U, NULL, osPriorityNormal, &hTM);


    //vTaskStartScheduler();
}
