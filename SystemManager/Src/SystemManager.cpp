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
        // if (this->rcInputs_.isDataNew){
        	watchdog_.refreshWatchdog();
        // }


        if(this->rcInputs_.arm >= (SBUS_MAX/2)) {
            this->throttleMotorChannel_.set(rcInputs_.throttle);
            this->yawMotorChannel_.set(rcInputs_.yaw);
            this->rollMotorChannel_.set(SBUS_MAX - rcInputs_.roll);
            this->pitchMotorChannel_.set(SBUS_MAX - rcInputs_.pitch);
            this->invertedRollMotorChannel_.set(SBUS_MAX - rcInputs_.roll);
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
