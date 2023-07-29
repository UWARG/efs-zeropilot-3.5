//
// Created by Gagan Deep Singh on 2023-07-08.
//

#ifndef ZEROPILOT_3_5_SYSTEMMANAGER_HPP
#define ZEROPILOT_3_5_SYSTEMMANAGER_HPP

#include "sbus_receiver.hpp"
#include "rcreceiver_datatypes.h"
#include "independent_watchdog.h"
#include "ZP_D_PWMChannel.hpp"

class SystemManager {
public:
    /* Constructors and Destructors */
    SystemManager();
    ~SystemManager();

    /* Class Functions */
    void flyManually();

private:
    /* Private helper functions */
    void updateRCInputs();
    void executeInputs();
    
    SBUSReceiver rcController_;
    RCControl rcInputs_;
    PWMChannel throttleMotorChannel_;
    PWMChannel yawMotorChannel_;
    PWMChannel rollMotorChannel_;
    PWMChannel pitchMotorChannel_;
    IndependentWatchdog watchdog_;
};


#endif //ZEROPILOT_3_5_SYSTEMMANAGER_HPP
