//
// Created by Gagan Deep Singh on 2023-07-08.
//

#include "../Inc/SystemManager.hpp"
#include "../../Models/testmodel1/config.hpp"
#include "../../Models/global_config.hpp"
#include "../../Drivers/rc_receiver/Inc/sbus_receiver.hpp"
#include "../../Drivers/rc_receiver/Inc/sbus_defines.h"
#include "../../Drivers/rc_receiver/Inc/rcreceiver_datatypes.h"
#include "../../Drivers/Common/Inc/drivers_config.hpp"

SystemManager::SystemManager():
    currentFlightMode_(config::flightmodes[0]),
    rcController_(sbus_uart)
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
    return;
}

config::Flightmode* SystemManager::getCurrentFlightMode() {
    return this->currentFlightMode_;
}

void SystemManager::setCurrentFlightMode(config::Flightmode *flightmode) {
    this->currentFlightMode_ = flightmode;
}

