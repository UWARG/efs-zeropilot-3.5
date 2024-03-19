#include "TelemetryManager.hpp"

/**
 * @brief This task is called every 500ms. It is responsible for
 * sending the highest priority/routine drone "state" data to the ground station. Data such as
 * heartbeat message, altitude, attitude, latitude, longitude... And anything else deemed
 * important enough to be transmitted at a regular interval. This is the highest priority
 * data in the GSC.highPriorityTransmitBuffer.
 *
 */
TelemetryTask* routineDataTransmission;

/**
 * @brief This thread is responsible for taking data from other managers and converting
 * them to Mavlink bytes, then putting them into GSC.lowPriorityTransmitBuffer.
 */
TelemetryTask* translateToMavlink;

/**
 * @brief This thread is responsible for taking the bytes from the GSC.DMAReceiveBuffer and
 * converting them to Mavlink messages/triggering the callbacks associated with each Mavlink
 * message.
 */
TelemetryTask* translateFromMavlink;

TelemetryManager::TelemetryManager() {
    this->MT = MavlinkTranslator();
    this->GSC = GroundStationCommunication();
}

TelemetryManager::~TelemetryManager() {
    // Destructor
    teardownTasks();
}

/**
 * @brief Initialize TM threads and timer interrupts.
 *
 */
void TelemetryManager::init() {
    // register TM tasks with FreeRTOS
    spinUpTasks();
}

void TelemetryManager::spinUpTasks() {
    routineDataTransmission =
        new TelemetryTask("routineDataTransmission", configMINIMAL_STACK_SIZE, tskIDLE_PRIORITY,
                          *this, [](TelemetryManager& tm) -> void {
                              auto GSC = tm.GSC;
                              while (true) {
                                  // START: ingest drone state data and pack bytes into
                                  // GSC.highPriorityTransmitBuffer

                                  // END: ingest drone state data and pack bytes into
                                  // GSC.highPriorityTransmitBuffer

                                  // transmit the data via GSC.transmit(); function
                                  GSC.transmit(GSC.highPriorityTransmitBuffer);

                                  // The interval at which the instructions in the lambda function
                                  // are executed.
                                  vTaskDelay(pdMS_TO_TICKS(500));  // Adjust the delay as necessary
                              }
                          });

    translateToMavlink =
        new TelemetryTask("translateToMavlink", configMINIMAL_STACK_SIZE, tskIDLE_PRIORITY, *this,
                          [](TelemetryManager& tm) -> void {
                              auto MT = tm.MT;
                              auto GSC = tm.GSC;

                              while (true) {
                                  MT.bytesToMavlinkMsg(GSC.DMAReceiveBuffer);
                                  vTaskDelay(pdMS_TO_TICKS(10));  // Adjust the delay as necessary
                              }
                          });

    translateFromMavlink =
        new TelemetryTask("translateFromMavlink", configMINIMAL_STACK_SIZE, tskIDLE_PRIORITY, *this,
                          [](TelemetryManager& tm) -> void {
                              while (true) {
                                  // START: fill GSC.lowPriorityTransmitBuffer with data to transmit

                                  // END: fill GSC.lowPriorityTransmitBuffer with data to transmit

                                  vTaskDelay(pdMS_TO_TICKS(10));  // Adjust the delay as necessary
                              }
                          });
}

void TelemetryManager::teardownTasks() {
    delete routineDataTransmission;
    delete translateToMavlink;
    delete translateFromMavlink;
}

/**
 * @brief This function is responsible for
 * sending non routine data to the ground station. Such as arm disarmed message status,
 * fulfilling data requests from the ground station etc. This is the lowest priority data
 * in the GSC.lowPriorityTransmitBuffer.
 */
void TelemetryManager::transmitNonRoutineData() {
    // transmit non routine data via GSC.transmit(); function
    GSC.transmit(GSC.lowPriorityTransmitBuffer);
}
