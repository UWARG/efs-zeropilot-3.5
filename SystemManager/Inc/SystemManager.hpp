//
// Created by Gagan Deep Singh on 2023-07-08.
//

#ifndef ZEROPILOT_3_5_SYSTEMMANAGER_HPP
#define ZEROPILOT_3_5_SYSTEMMANAGER_HPP

#include "sbus_receiver.hpp"
#include "rcreceiver_datatypes.h"
#include "independent_watchdog.h"
#include "ZP_D_PWMChannel.hpp"
#include "TelemetryManager.hpp"

#define SBUS_THRESHOLD 25
#define SBUS_MIN 0
#define SBUS_MAX 100

class SystemManager {
    public:
        /* Constructors and Destructors */
        SystemManager();

        /* Other managers*/
        TelemetryManager *telemetryManager;

        TelemetryManager* setupTM();

        /* Class Functions */
        void startSystemManager(); 
        
    private:
        SBUSReceiver* rcController_;
        RCControl rcInputs_;
        PWMChannel throttleMotorChannel_;
        PWMChannel yawMotorChannel_;
        PWMChannel rollMotorChannel_;
        PWMChannel invertedRollMotorChannel_;
        PWMChannel pitchMotorChannel_;
        IndependentWatchdog watchdog_;

        static void attitudeManagerTaskWrapper(void *pvParameters);
        static void telemetryManagerTaskWrapper(void *pvParameters);
        static void pathManagerTaskWrapper(void *pvParameters);
        
        void systemManagerTask();
        void attitudeManagerTask();
        void telemetryManagerTask();
        void pathManagerTask();
};

#endif //ZEROPILOT_3_5_SYSTEMMANAGER_HPP
