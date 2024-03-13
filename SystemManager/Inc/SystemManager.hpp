//
// Created by Gagan Deep Singh on 2023-07-08.
//

#ifndef ZEROPILOT_3_5_SYSTEMMANAGER_HPP
#define ZEROPILOT_3_5_SYSTEMMANAGER_HPP

#include "sbus_receiver.hpp"
#include "rcreceiver_datatypes.h"
#include "independent_watchdog.h"
#include "ZP_D_PWMChannel.hpp"

#define SBUS_THRESHOLD 25
#define SBUS_MIN 0
#define SBUS_MAX 100

class SystemManager {
public:
    /* Constructors and Destructors */
    SystemManager();
    ~SystemManager();

    /* Class Functions */
    void setup();
    

private:
    void systemCheckTask(void *pvParameters);
    static void systemCheckTaskWrapper(void* pvParameters);
    
    void attitudeManagerTask(void *pvParameters);
    static void attitudeManagerTaskWrapper(void* pvParameters);

    void telemetryManagerTask(void *pvParameters);
    static void telemetryManagerTaskWrapper(void* pvParameters);
    SBUSReceiver* rcController_;
    RCControl rcInputs_;
    PWMChannel throttleMotorChannel_;
    PWMChannel yawMotorChannel_;
    PWMChannel rollMotorChannel_;
    PWMChannel invertedRollMotorChannel_;
    PWMChannel pitchMotorChannel_;
    IndependentWatchdog watchdog_;
};


#endif //ZEROPILOT_3_5_SYSTEMMANAGER_HPP
