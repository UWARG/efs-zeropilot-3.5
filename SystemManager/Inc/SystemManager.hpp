//
// Created by Gagan Deep Singh on 2023-07-08.
//

#ifndef ZEROPILOT_3_5_SYSTEMMANAGER_HPP
#define ZEROPILOT_3_5_SYSTEMMANAGER_HPP

#include "sbus_receiver.hpp"
#include "rcreceiver_datatypes.h"
#include "independent_watchdog.h"
#include "ZP_D_PWMChannel.hpp"
#include "AM.hpp"

#define SBUS_THRESHOLD 25
#define SBUS_MIN 0
#define SBUS_MAX 100
#define AM_PERIOD_MS 5

class SystemManager {
public:
    /* Constructors and Destructors */
    SystemManager();
    ~SystemManager();

    /* Class Functions */
    void flyManually();
    static void runAM(void* pvParameters);

private:
    SBUSReceiver rcController_;
    RCControl rcInputs_;
    PWMChannel throttleMotorChannel_;
    PWMChannel yawMotorChannel_;
    PWMChannel rollMotorChannel_;
    PWMChannel pitchMotorChannel_;
    IndependentWatchdog watchdog_;
    AM::AttitudeManager am_instance_;
    TaskHandle_t AM_handle_ = NULL;
};


#endif //ZEROPILOT_3_5_SYSTEMMANAGER_HPP
