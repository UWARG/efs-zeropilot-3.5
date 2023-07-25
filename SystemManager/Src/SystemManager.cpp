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
#include "../../Drivers/IWDG_driver/Inc/independent_watchdog.h"


SystemManager::SystemManager():
    rcController_(sbus_uart),
    watchdog_(watchdog_reference)
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
    /* Update AM's static variable and call function to run control loop */
    return;
}

