#include "FreeRTOS.h"
#include "GroundStationComms.hpp"
#include "MavlinkTranslator.hpp"
#include "TimerInterrupt.hpp"
#include "task.h"

TelemetryManager::TelemetryManager() {
    this->MT = MavlinkTranslator();
    this->GSC = GroundStationComms();
}

TelemetryManager::~TelemetryManager() {
    // Destructor
}

/**
 * @brief Initialize TM threads and timer interrupts.
 *
 */
void TelemetryManager::init() {
    // initialize the timer interrupts
    initTimerInterrupts();

    // Init TM tasks
    initTasks();

    // Start the scheduler
    vTaskStartScheduler();  // should this be in system manager instead?
}

void TelemetryManager::initTimerInterrupts() {
    /**
     * @brief This interrupt service routine is called every 1000ms. It is responsible for
     * sending the highest priority drone "state" data to the ground station. Data such as
     * heartbeat message, altitude, attitude, latitude, longitude... And anything else deemed
     * important enough to be transmitted at a regular interval. This is the highest priority
     * data in the GSC.highPriorityTransmitBuffer.
     *
     */
    TimerInterrupt dispatchHighPriorityInterrupt1000Ms(
        "dispatchHighPriority1000Ms", configMINIMAL_STACK_SIZE, tskIDLE_PRIORITY, 500, *this,
        [](TelemetryManager& tm) -> void {
            auto GSC = tm.GSC;

            // START: ingest drone state data and pack bytes into GSC.highPriorityTransmitBuffer

            // END: ingest drone state data and pack bytes into GSC.highPriorityTransmitBuffer

            // transmit the data via GSC.sendToGroundStation(); function
            GSC.sendToGroundStation(GSC.highPriorityTransmitBuffer);
        });
}

void TelemetryManager::initTasks() {
    // Initialize translateToMavlinkThread as a FreeRTOS task
    xTaskCreate(TelemetryManager::translateToMavlinkThreadStatic,  // Task function
                "translateToMavlinkThread",                        // Task name
                configMINIMAL_STACK_SIZE,                          // Stack size
                this,                                              // Task parameters
                tskIDLE_PRIORITY + 1,                              // Task priority
                NULL                                               // Task handle
    );

    // Initialize mavlinkToBytesThread as a FreeRTOS task
    xTaskCreate(TelemetryManager::mavlinkToBytesThreadStatic,  // Task function
                "mavlinkToBytesThread",                        // Task name
                configMINIMAL_STACK_SIZE,                      // Stack size
                this,                                          // Task parameters
                tskIDLE_PRIORITY + 1,                          // Task priority
                NULL                                           // Task handle
    );
}

/**
 * @brief This thread is responsible for taking the bytes from the GSC.DMAReceiveBuffer and
 * converting them to Mavlink messages/triggering the callbacks associated with each Mavlink
 * message.
 */
void TelemetryManager::translateToMavlinkThread() {
    while (true) {
        MT.bytesToMavlinkMsg(GSC.DMAReceiveBuffer);

        vTaskDelay(pdMS_TO_TICKS(10));  // Adjust the delay as necessary
    }
}

/**
 * @brief This thread is responsible for taking data from other managers and converting
 * them to Mavlink bytes, then putting them into GSC.lowPriorityTransmitBuffer.
 */
void TelemetryManager::mavlinkToBytesThread() {
    while (true) {
        // START: fill GSC.lowPriorityTransmitBuffer with data to transmit

        // END: fill GSC.lowPriorityTransmitBuffer with data to transmit

        vTaskDelay(pdMS_TO_TICKS(10));  // Adjust the delay as necessary
    }
}

/**
 * @brief This function is responsible for
 * sending non routine data to the ground station. Such as arm disarmed message status,
 * fulfilling data requests from the ground station etc. This is the lowest priority data
 * in the GSC.lowPriorityTransmitBuffer.
 */
void TelemetryManager::sendLowPriorityData() {
    // transmit low priority the data via GSC.sendToGroundStation(); function
    GSC.sendToGroundStation(GSC.lowPriorityTransmitBuffer);
}
