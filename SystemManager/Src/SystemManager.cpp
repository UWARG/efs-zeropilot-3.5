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
#include "CommonDataTypes.hpp"

SystemManager::SystemManager():
    rcController_(&huart2),
    throttleMotorChannel_(&htim2, TIM_CHANNEL_1),
    yawMotorChannel_(&htim2, TIM_CHANNEL_2),
    rollMotorChannel_(&htim2, TIM_CHANNEL_3),
    pitchMotorChannel_(&htim2, TIM_CHANNEL_4),
    watchdog_(&hiwdg),
    am_instance_(nullptr) // TODO: default flight mode? 
{
    // TODO: define priority levels
    xTaskCreate(runAM, "AM Thread", 400U, (void*)&am_instance_, (configMAX_PRIORITIES / 2), &AM_handle_);
}

SystemManager::~SystemManager() {
    vTaskDelete(AM_handle_);
}

void SystemManager::runAM(void* pvParameters) {
    AM::AttitudeManager* am_instance = (AM::AttitudeManager *) pvParameters;
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    for(;;) {
        am_instance->runControlLoopIteration(AM::AttitudeManager::getControlInputs());
        vTaskDelayUntil(&xLastWakeTime, AM_PERIOD_MS);
    }
}

void SystemManager::flyManually() {
    for(;;){
        this->rcInputs_ = rcController_.GetRCControl();
        if (this->rcInputs_.isDataNew) watchdog_.refreshWatchdog();
        
        AM::AttitudeManagerInput am_input = {
            .roll = 0,
            .pitch = 0,
            .yaw = 0,
            .throttle = 0
        };

        if (this->rcInputs_.arm >= (SBUS_MAX/2)) {
            am_input.roll = rcInputs_.roll;
            am_input.pitch = rcInputs_.pitch;
            am_input.yaw = rcInputs_.yaw;
            am_input.throttle = rcInputs_.throttle;
            AM::AttitudeManager::setControlInputs(am_input);
        } else {
            AM::AttitudeManager::setControlInputs(am_input);
        }
    }
}
