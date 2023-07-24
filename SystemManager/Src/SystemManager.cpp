//
// Created by Gagan Deep Singh on 2023-07-08.
// Edited by Navtaj Hundal on 2023-07-23.
//

#include "../Inc/SystemManager.hpp"
#include "../../Models/testmodel1/config.hpp"
#include "../../Models/global_config.hpp"
#include "../../Drivers/rc_receiver/Inc/sbus_receiver.hpp"
#include "../../Drivers/rc_receiver/Inc/sbus_defines.h"
#include "../../Drivers/rc_receiver/Inc/rcreceiver_datatypes.h"
#include "../../Drivers/Common/Inc/drivers_config.hpp"
#include "../../Boardfiles/nucleol552zeq/Core/Src/main.c"

SystemManager::SystemManager():
    currentFlightMode_(config::flightmodes[0]),
    rcController_(sbus_uart),
    throttleMotorChannel_(&htim2, TIM_CHANNEL_1);
    yawMotorChannel_(&htim2, TIM_CHANNEL_2);
    rollMotorChannel_(&htim2, TIM_CHANNEL_3);
    pitchMotorChannel_(&htim2, TIM_CHANNEL_4);
    {}/* Would need to instantiate SBUS, IWDG, and AM */
SystemManager::~SystemManager() {}

void SystemManager::flyManually() {
    for(;;){
            updateRCInputs();
            executeInputs();
    }
}

void SystemManager::updateRCInputs() {
    this->rcInputs_.assignValues(rcController_.GetRCControl());
    return;
}

void SystemManager::executeInputs() {
    /* Update AM's static variable and call function to run control loop */
    throttleMotorChannel_.set(rcInputs_.throttle);
    yawMotorChannel_.set(rcInputs_.yaw);
    rollMotorChannel_.set(rcInputs_.roll);
    pitchtMotorChannel_.set(rcInputs_.pitch);
    return;
}

config::Flightmode* SystemManager::getCurrentFlightMode() {
    return this->currentFlightMode_;
}

void SystemManager::setCurrentFlightMode(config::Flightmode *flightmode) {
    this->currentFlightMode_ = flightmode;
}
