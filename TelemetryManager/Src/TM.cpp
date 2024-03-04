#include "TM.hpp"

TelemetryManager::~TelemetryManager() {
    // Destructor
}

void TelemetryManager::init() {
    GroundStationComms GSC = getInstance().GSC;
    TimerInterrupt TI = getInstance().TI;
    MavlinkTranslator MT = getInstance().MT;

    // initialize the timer interrupts
    TI.registerTimerInterrupt(1000, TimerISR1000ms);

    // Initialize translateToMavlinkThread as a FreeRTOS task
    xTaskCreate(mavlinkToBytesThread,        // Task function
                "translateToMavlinkThread",  // Task name
                configMINIMAL_STACK_SIZE,    // Stack size
                NULL,                        // Task parameters
                tskIDLE_PRIORITY + 1,        // Task priority
                NULL                         // Task handle
    );

    // Initialize mavlinkToBytesThread as a FreeRTOS task
    xTaskCreate(mavlinkToBytesThread,      // Task function
                "mavlinkToBytesThread",    // Task name
                configMINIMAL_STACK_SIZE,  // Stack size
                NULL,                      // Task parameters
                tskIDLE_PRIORITY + 1,      // Task priority
                NULL                       // Task handle
    );

    // Start the scheduler
    vTaskStartScheduler();  // should this be in system manager instead?
}

void TelemetryManager::translateToMavlinkThread(void *pvParameters) {
    // give access to any of the TM's members statically
    MavlinkTranslator MT = getInstance().MT;
    GroundStationComms GSC = getInstance().GSC;

    while (true) {
        MT.bytesToMavlinkMsg(GSC.DMAReceiveBuffer);

        vTaskDelay(pdMS_TO_TICKS(10));  // Adjust the delay as necessary
    }
}

void TelemetryManager::mavlinkToBytesThread(void *pvParameters) {
    while (true) {
        // START: fill GSC.lowPriorityTransmitBuffer with data to transmit

        // END: fill GSC.lowPriorityTransmitBuffer with data to transmit

        vTaskDelay(pdMS_TO_TICKS(10));  // Adjust the delay as necessary
    }
}

void TelemetryManager::sendLowPriorityData() {
    // there will be some kind of trigger for this function to be called. not sure what it is yet.

    GroundStationComms GSC = getInstance().GSC;
    // transmit low priority the data via GSC.sendToGroundStation(); function
    GSC.sendToGroundStation(GSC.lowPriorityTransmitBuffer);
}

void TelemetryManager::TimerISR1000ms() {
    // give access to any of the TM's members statically
    GroundStationComms GSC = getInstance().GSC;

    // START: ingest drone state data and pack bytes into GSC.highPriorityTransmitBuffer

    // END: ingest drone state data and pack bytes into GSC.highPriorityTransmitBuffer

    // transmit the data via GSC.sendToGroundStation(); function
    GSC.sendToGroundStation(GSC.highPriorityTransmitBuffer);
}