
#include "GroundStationComs.hpp"
#include "MavlinkTranslator.hpp"
#include "TimerInterrupt.hpp"

#include "FreeRTOS.h"
#include "task.h"


GroundStationComms GSC;

MavlinkTranslator MT;

TimerInterrupt TI;

// Forward declaration of the task functions
void translateToMavlinkThread(void *pvParameters);
void mavlinkToBytesThread(void *pvParameters);


/**
 * @brief Initialize TM threads and timer interrupts. 
 * 
 */
void init()
{

    // initialize the timer interrupts
    TI.registerTimerInterrupt(500, TimerISR500ms);
    TI.registerTimerInterrupt(1000, TimerISR1000ms);

    // Initialize translateToMavlinkThread as a FreeRTOS task
    xTaskCreate(
        translateToMavlinkThread,                  // Task function
        "translateToMavlinkThread",                // Task name
        configMINIMAL_STACK_SIZE, // Stack size
        NULL,                     // Task parameters
        tskIDLE_PRIORITY + 1,     // Task priority
        NULL                      // Task handle
    );

    // Initialize mavlinkToBytesThread as a FreeRTOS task
    xTaskCreate(
        mavlinkToBytesThread,                  // Task function
        "mavlinkToBytesThread",                // Task name
        configMINIMAL_STACK_SIZE, // Stack size
        NULL,                     // Task parameters
        tskIDLE_PRIORITY + 1,     // Task priority
        NULL                      // Task handle
    );

    // Start the scheduler
    vTaskStartScheduler();
}

/**
 * @brief This thread is responsible for taking the bytes from the GSC.DMAReceiveBuffer and
 * converting them to Mavlink messages/triggering the callbacks associated with each mavlink
 * message.
 */
void translateToMavlinkThread()
{
    while (true)
    {
        MT.bytesToMavlinkMsg(GSC.DMAReceiveBuffer);

        vTaskDelay(pdMS_TO_TICKS(10)); // Adjust the delay as necessary
    }
}

/**
 * @brief This thread is responsible for taking data from other managers and converting
 * them to mavlink bytes, then putting them into GSC.lowPriorityTransmitBuffer.
 */
void mavlinkToBytesThread()
{
    while (true)
    {
        // START: fill GSC.lowPriorityTransmitBuffer with data to transmit

        // END: fill GSC.lowPriorityTransmitBuffer with data to transmit

        vTaskDelay(pdMS_TO_TICKS(10)); // Adjust the delay as necessary
    }
}

/**
 * @brief This interrupt service routine is called every 500ms. It is responsible for
 * sending non routine data to the ground station. Such as arm disarmed message status,
 * fulfilling data requests from the ground station etc. This is the lowest priority data
 * in the GSC.lowPriorityTransmitBuffer.
 */
void TimerISR500ms()
{
    // transmit low priority the data via GSC.sendToGroundStation(); function
    GSC.sendToGroundStation(GSC.lowPriorityTransmitBuffer);
}

/**
 * @brief This interrupt service routine is called every 1000ms. It is responsible for
 * sending the highest priority drone "state" data to the ground station. Data such as
 * heartbeat message, altitude, attitude, latitude, longitude... And anything else deemed
 * important enough to be transmitted at a regular interval. This is the highest priority
 * data in the GSC.highPriorityTransmitBuffer.
 *
 */
void TimerISR1000ms()
{

    // START: ingest drone state data and pack bytes into GSC.highPriorityTransmitBuffer

    // END: ingest drone state data and pack bytes into GSC.highPriorityTransmitBuffer

    // transmit the data via GSC.sendToGroundStation(); function
    GSC.sendToGroundStation(GSC.highPriorityTransmitBuffer);
}