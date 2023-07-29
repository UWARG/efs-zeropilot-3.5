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
    rcController_(&huart2),
    throttleMotorChannel_(&htim2, TIM_CHANNEL_1),
    yawMotorChannel_(&htim2, TIM_CHANNEL_2),
    rollMotorChannel_(&htim2, TIM_CHANNEL_3),
    pitchMotorChannel_(&htim2, TIM_CHANNEL_4),
    watchdog_(&hiwdg)
{}
SystemManager::~SystemManager() {}

void SystemManager::flyManually() {
    for(;;){
            updateRCInputs();
            executeInputs();
    }
}

void SystemManager::updateRCInputs() {
    this->rcInputs_.assignValues(rcController_.GetRCControl());
    watchdog_.refreshWatchdog();
    return;
}

void SystemManager::executeInputs() {
    this->throttleMotorChannel_.set(rcInputs_.throttle);
    this->yawMotorChannel_.set(rcInputs_.yaw);
    this->rollMotorChannel_.set(rcInputs_.roll);
    this->pitchMotorChannel_.set(rcInputs_.pitch);
    return;
}
