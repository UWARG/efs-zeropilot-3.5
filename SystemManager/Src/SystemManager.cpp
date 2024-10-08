//
// Created by Gagan Deep Singh on 2023-07-08.
// Edited by Navtaj Hundal on 2023-07-23.
//

#include "SystemManager.hpp"

#include "drivers_config.hpp"
#include "independent_watchdog.h"
#include "rcreceiver_datatypes.h"
#include "sbus_defines.h"
#include "sbus_receiver.hpp"
#include "tim.h"


#define TIMEOUT_CYCLES 250000 // 25k = 1 sec fro testing 10/14/2023 => 250k = 10 sec
#define TIMOUT_MS      10000 // 10 sec

static uint32_t DisconnectionCount = 0;
float prevthrottle;
float prevyaw;
float prevroll;
float prevpitch;

SystemManager::SystemManager()
    : rcController_(sbus_pointer),
      throttleMotorChannel_(&htim2, TIM_CHANNEL_1),
      yawMotorChannel_(&htim2, TIM_CHANNEL_2),
      rollMotorChannel_(&htim2, TIM_CHANNEL_3),
      pitchMotorChannel_(&htim2, TIM_CHANNEL_4),
      invertedRollMotorChannel_(&htim3, TIM_CHANNEL_1),
      watchdog_(TIMOUT_MS) {
    // VARIABLES FOR TELEMETRY MANAGER TO HAVE AS REFERENCES THEY OBV SHOULD BE PUT SOMEWHERE ELSE,
    // BUT I FEEL LIKE SM PM WOULD KNOW WHERE. MAYBE IN THE HPP FILE? IDK HOW YOU ARE PLANNING ON
    // GATHERING THE DATA. I JUST PUT THEM HERE FOR NOW
    int32_t lat = 0;
    int32_t lon = 0;
    int32_t alt = 0;
    int32_t relative_alt = 0;
    int16_t vx = 0;
    int16_t vy = 0;
    int16_t vz = 0;
    uint16_t hdg = 0;
    int32_t time_boot_ms = 0;
    MAV_STATE state = MAV_STATE::MAV_STATE_STANDBY;
    MAV_MODE_FLAG mode = MAV_MODE_FLAG::MAV_MODE_FLAG_MANUAL_INPUT_ENABLED;
    float roll = 0;
    float pitch = 0;
    float yaw = 0;
    float rollspeed = 0;
    float pitchspeed = 0;
    float yawspeed = 0;

    this->telemetryManager =
        new TelemetryManager(lat, lon, alt, relative_alt, vx, vy, vz, hdg, time_boot_ms, state,
                             mode, roll, pitch, yaw, rollspeed, pitchspeed, yawspeed);
    this->telemetryManager->init();
    // IDK WHERE SM PLANS TO DO THIS, BUT telemetryManager.update() NEEDS TO BE CALLED AT A SEMI
    // REGULAR INTERVAL AS IT DEALS WITH MESSAGE DECODING AND LOW PRIORITY DATA TRANSMISSION
}


SystemManager::~SystemManager() {}

void SystemManager::flyManually() {
    for (;;) {
        this->rcInputs_ = rcController_->GetRCControl();

        // TO-DO: need to implement it using is_Data_New;
        //  boolean is true if data has not changed since the last cycle
        bool is_unchanged{rcInputs_.throttle == prevthrottle && rcInputs_.yaw == prevyaw &&
                          rcInputs_.roll == prevroll && rcInputs_.pitch == prevpitch};

        if (is_unchanged) {
            DisconnectionCount += 1;  // if its not changed we increment the timeout counter
            if (DisconnectionCount > TIMEOUT_CYCLES) {  // if timeout has occured
                DisconnectionCount =
                    TIMEOUT_CYCLES + 1;   // avoid overflow but keep value above threshold
                this->rcInputs_.arm = 0;  // failsafe
            }
        } else {
            DisconnectionCount = 0;  // if the data has changed we want to reset out counter
        }

        watchdog_.refreshWatchdog();  // always hit the dog

        if (this->rcInputs_.arm >= (SBUS_MAX / 2)) {
            this->throttleMotorChannel_.set(rcInputs_.throttle);
            this->yawMotorChannel_.set(rcInputs_.yaw);
            this->rollMotorChannel_.set(SBUS_MAX - rcInputs_.roll);
            this->pitchMotorChannel_.set(SBUS_MAX - rcInputs_.pitch);
            this->invertedRollMotorChannel_.set(SBUS_MAX - rcInputs_.roll);

            prevthrottle = rcInputs_.throttle;
            prevyaw = rcInputs_.yaw;
            prevroll = rcInputs_.roll;
            prevpitch = rcInputs_.pitch;

        } else {
            this->throttleMotorChannel_.set(0);
            this->yawMotorChannel_.set(SBUS_MAX / 2);
            this->rollMotorChannel_.set(SBUS_MAX / 2);
            this->pitchMotorChannel_.set(SBUS_MAX / 2);
            this->invertedRollMotorChannel_.set(SBUS_MAX / 2);
        }
    }
}
