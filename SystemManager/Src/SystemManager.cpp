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
#include <stdio.h>

#define DATANEW_TIMEOUT 75

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

void SystemManager::flyManually() {
    for(;;){

        this->rcInputs_ = rcController_->GetRCControl();

        printf("Arm: %f\r\n", this->rcInputs_.arm);

        //Is_Data_new implementation for failsafe
        if (!this->rcInputs_.isDataNew){ //if the data is not new

        	DisconnectionCount += 1; //increment the counter
        	if (DisconnectionCount > DATANEW_TIMEOUT){ //if the counter is greater than 75, then we can disarm
        		this->rcInputs_.arm = 0; //disarm the drone for failsafe
        	}
        }
        else{
        	DisconnectionCount = 0; //if the data is new, then we reset to 0.
        }

       // watchdog_.refreshWatchdog(); // always hit the dog

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
