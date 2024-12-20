//
// Created by Gagan Deep Singh on 2023-07-08.
// Edited by Navtaj Hundal on 2023-07-23.
//

#include "SystemManager.hpp"

#include <iostream>

#include "FreeRTOS.h"
#include "GroundStationCommunication.hpp"
#include "TelemetryManager.hpp"
#include "cmsis_os.h"
#include "drivers_config.hpp"
#include "independent_watchdog.h"
#include "rcreceiver_datatypes.h"
#include "sbus_defines.h"
#include "sbus_receiver.hpp"
#include "tim.h"

extern "C" {
#include "app_fatfs.h"
#include "log_util.h"
}

#define DATANEW_TIMEOUT 75
#define TIMEOUT_CYCLES 250000  // 25k = 1 sec fro testing 10/14/2023 => 250k = 10 sec
#define TIMOUT_MS 10000        // 10 sec

// 0 - AM, 1 - TM, 2 - PM
static TaskHandle_t taskHandles[3];

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

   
    this->telemetryManager = setupTM();
    this->telemetryManager->init();

    // IDK WHERE SM PLANS TO DO THIS, BUT telemetryManager.update() NEEDS TO BE CALLED AT A SEMI
    // REGULAR INTERVAL AS IT DEALS WITH MESSAGE DECODING AND LOW PRIORITY DATA TRANSMISSION    
}

TelemetryManager* SystemManager::setupTM() {

     // Struct containing the state of the drone
    TMStateData stateData;

    // values to be assigned to stateData
    // THIS IS JUST A PLACEHOLDER, THE MEMORY ADDRESSES NEED TO LIVE LONGER THAN THE SM CONSTRUCTOR
    int32_t alt = 0;
    int32_t lat = 0;
    int32_t lon = 0;
    int32_t relative_alt = 0;
    int16_t vx = 0;
    int16_t vy = 0;
    int16_t vz = 0;
    uint16_t hdg = 0;
    int32_t time_boot_ms = 0;
    float roll = 0;
    float pitch = 0;
    float yaw = 0;
    float rollspeed = 0;
    float pitchspeed = 0;
    float yawspeed = 0;

    // use the memory address of the above, since stateData uses pointers
    stateData.set_alt(&alt);
    stateData.set_lat(&lat);
    stateData.set_lon(&lon);
    stateData.set_relative_alt(&relative_alt);
    stateData.set_vx(&vx);
    stateData.set_vy(&vy);
    stateData.set_vz(&vz);
    stateData.set_hdg(&hdg);
    stateData.set_time_boot_ms(&time_boot_ms);
    stateData.set_roll(&roll);
    stateData.set_pitch(&pitch);
    stateData.set_yaw(&yaw);
    stateData.set_rollspeed(&rollspeed);
    stateData.set_pitchspeed(&pitchspeed);
    stateData.set_yawspeed(&yawspeed);

    MAV_STATE mavState = MAV_STATE::MAV_STATE_STANDBY;
    MAV_MODE_FLAG mavMode = MAV_MODE_FLAG::MAV_MODE_FLAG_MANUAL_INPUT_ENABLED;

    // Creating parameters for the GroundStationCommunication that will be passed to
    // telemetryManager
    TMCircularBuffer DMAReceiveBuffer = *(new TMCircularBuffer(rfd900_circular_buffer));

    // the buffer that stores non_routine/low_priority bytes (ex. Battery Voltage) to be sent to the
    // ground station.
    uint8_t *lowPriorityTransmitBuffer = new uint8_t[RFD900_BUF_SIZE];

    // the buffer that stores routine/high_priority bytes (ex. heading, general state data) to be
    // sent to the ground station.
    uint8_t *highPriorityTransmitBuffer = new uint8_t[RFD900_BUF_SIZE];

    GroundStationCommunication GSC = *(new GroundStationCommunication(
        DMAReceiveBuffer, lowPriorityTransmitBuffer, highPriorityTransmitBuffer, RFD900_BUF_SIZE));

    // the buffer that stores the bytes received from the ground station.
    MavlinkTranslator MT;

    
    return new TelemetryManager(stateData, mavState, mavMode, GSC, MT);
}

// wrapper functions are needed as FreeRTOS xTaskCreate function does not accept functions that have
// "this" pointers
void SystemManager::attitudeManagerTaskWrapper(void *pvParameters) {
    SystemManager *systemManagerInstance = static_cast<SystemManager *>(pvParameters);
    systemManagerInstance->attitudeManagerTask();
}

void SystemManager::telemetryManagerTaskWrapper(void *pvParameters) {
    SystemManager *systemManagerInstance = static_cast<SystemManager *>(pvParameters);
    systemManagerInstance->telemetryManagerTask();
}

void SystemManager::pathManagerTaskWrapper(void *pvParameters) {
    SystemManager *systemManagerInstance = static_cast<SystemManager *>(pvParameters);
    systemManagerInstance->pathManagerTask();
}

void SystemManager::systemManagerTask() {
    TickType_t xNextWakeTime = xTaskGetTickCount();
    uint16_t frequency = 5;

    for (;;) {
        printf("SM called\r\n");
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);  // toggle led light for testing

        this->rcInputs_ = rcController_->GetRCControl();

        //Is_Data_new implementation for failsafe
		if (!this->rcInputs_.isDataNew){ //if the data is not new

			DisconnectionCount += 1; //increment the counter
			if (DisconnectionCount > DATANEW_TIMEOUT){ //if the counter is greater than 75, then we can disarm
				this->rcInputs_.arm = 0; //disarm the drone for failsafe
			}
		}
		else{
			DisconnectionCount = 0; //if the data is new, then we reset to 0.
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

        vTaskDelayUntil(&xNextWakeTime, 1000 / frequency);
    }
}

void SystemManager::attitudeManagerTask() {
    TickType_t xNextWakeTime = xTaskGetTickCount();
    uint16_t frequency = 5;

    for (;;) {
        // call AM

        printf("AM called\r\n");
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);  // toggle led light for testing
        vTaskDelayUntil(&xNextWakeTime, 1000 / frequency);
    }
}

void SystemManager::telemetryManagerTask() {
    TickType_t xNextWakeTime = xTaskGetTickCount();
    uint16_t frequency = 5;

    for (;;) {
        // call TM

        printf("TM called\r\n");
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);  // toggle led light for testing
        vTaskDelayUntil(&xNextWakeTime, 1000 / frequency);
    }
}

void SystemManager::pathManagerTask() {
    TickType_t xNextWakeTime = xTaskGetTickCount();
    uint16_t frequency = 5;

    for (;;) {
        // call PM

        printf("PM called\r\n");
        vTaskDelayUntil(&xNextWakeTime, 1000 / frequency);
    }
}

void SystemManager::startSystemManager() {
    printf("Initializing Tasks\r\n");

    // enabling SD card logging
    if (MX_FATFS_Init() != APP_OK) {
        Error_Handler();
    }
    logInit();

    // BaseType_t xTaskCreate( TaskFunction_t pvTaskCode, const char * const pcName,
    // configSTACK_DEPTH_TYPE usStackDepth, void * pvParameters, UBaseType_t uxPriority,
    // TaskHandle_t * pxCreatedTask );
    //                           function's name             description                 size of
    //                           stack to allocate        parameters for task        priority
    //                           handler
    xTaskCreate(attitudeManagerTaskWrapper, "AM TASK", 800U, this, osPriorityNormal, taskHandles);
    xTaskCreate(telemetryManagerTaskWrapper, "TM TASK", 800U, this, osPriorityNormal,
                taskHandles + 1);
    // xTaskCreate(pathManagerTaskWrapper, "PM TASK", 800U, this, osPriorityNormal, taskHandles +2);

    systemManagerTask();
}
