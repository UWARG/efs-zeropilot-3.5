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
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include <iostream>

extern "C" {
  #include "app_fatfs.h"
  #include "log_util.h"
}

#define TIMEOUT_CYCLES 250000 // 25k = 1 sec fro testing 10/14/2023 => 250k = 10 sec
#define TIMOUT_MS      10000 // 10 sec

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


//wrapper functions are needed as FreeRTOS xTaskCreate function does not accept functions that have "this" pointers
void SystemManager::attitudeManagerTaskWrapper(void* pvParameters){
    SystemManager *systemManagerInstance = static_cast<SystemManager *>(pvParameters);  
    systemManagerInstance->attitudeManagerTask();
}

void SystemManager::telemetryManagerTaskWrapper(void* pvParameters){
    SystemManager *systemManagerInstance = static_cast<SystemManager *>(pvParameters);
    systemManagerInstance->telemetryManagerTask();
}

void SystemManager::pathManagerTaskWrapper(void *pvParameters) {
    SystemManager *systemManagerInstance = static_cast<SystemManager *>(pvParameters);
    systemManagerInstance->pathManagerTask();
}

void SystemManager::systemManagerTask(){
    TickType_t xNextWakeTime = xTaskGetTickCount();
    uint16_t frequency = 5;

    for(;;){
        printf("SM called\r\n");
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7); // toggle led light for testing

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

        vTaskDelayUntil(&xNextWakeTime, 1000 / frequency); 
    }
}

void SystemManager::attitudeManagerTask(){
    TickType_t xNextWakeTime = xTaskGetTickCount();
    uint16_t frequency = 5;

    for(;;){
        //call AM

        printf("AM called\r\n");
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7); // toggle led light for testing
        vTaskDelayUntil(&xNextWakeTime, 1000 / frequency);  
    }
}

void SystemManager::telemetryManagerTask(){
    TickType_t xNextWakeTime = xTaskGetTickCount();
    uint16_t frequency = 5;

    for(;;){
        //call TM

        printf("TM called\r\n");
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9); // toggle led light for testing
        vTaskDelayUntil(&xNextWakeTime, 1000 / frequency);  
    }
}

void SystemManager::pathManagerTask(){
    TickType_t xNextWakeTime = xTaskGetTickCount();
    uint16_t frequency = 5;

    for(;;){
        //call PM

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

    //BaseType_t xTaskCreate( TaskFunction_t pvTaskCode, const char * const pcName, configSTACK_DEPTH_TYPE usStackDepth, void * pvParameters, UBaseType_t uxPriority, TaskHandle_t * pxCreatedTask ); 
    //                          function's name             description                 size of stack to allocate        parameters for task        priority                    handler 
    xTaskCreate(attitudeManagerTaskWrapper, "AM TASK", 800U, this, osPriorityNormal, taskHandles);
    xTaskCreate(telemetryManagerTaskWrapper, "TM TASK", 800U, this, osPriorityNormal, taskHandles + 1);
    // xTaskCreate(pathManagerTaskWrapper, "PM TASK", 800U, this, osPriorityNormal, taskHandles +2);

    systemManagerTask();

}
