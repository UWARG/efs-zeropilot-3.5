//
// Created by Gagan Deep Singh on 2023-07-08.
//

#include "SystemManager.hpp"
#include "global_config.hpp"
#include "config.hpp"
#include "sbus_reciever.hpp"
#include "drivers_config.hpp"


SystemManager::SystemManager():
    currentFlightMode_(config::flightmodes[0]),
    {}/* Would need to instantiate SBUS, IWDG, and AM */
SystemManager::~SystemManager() {}

void SystemManager::flyManually() {
    for(;;){
        try{
            updateRCInputs();
            executeInputs();
        }
        catch{
            break;
        }
    }
}

void SystemManager::updateRCInputs() {
    /* Get Inputs from SBUS and update corresponding private variable (also monitors Watchdog) */
    return;
}

void SystemManager::executeInputs() {
    /* Update AM's static variable and call function to run control loop */
    return;
}

Flightmode SystemManager::getCurrentFlightMode() {
    return this->currentFlightMode_;
}

void SystemManager::setCurrentFlightMode(config::Flightmode flightmode) {
    this->currentFlightMode_ = flightmode;
}

