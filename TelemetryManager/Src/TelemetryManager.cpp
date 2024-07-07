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

TelemetryManager::TelemetryManager(uint8_t& altitude)
    : DMAReceiveBuffer(new TMCircularBuffer(rfd900_circular_buffer)),
      lowPriorityTransmitBuffer(new uint8_t[RFD900_BUF_SIZE]),
      highPriorityTransmitBuffer(new uint8_t[RFD900_BUF_SIZE]),
      GSC(*DMAReceiveBuffer, lowPriorityTransmitBuffer, highPriorityTransmitBuffer,
          RFD900_BUF_SIZE),
      MT(),
      altitude(altitude) {}

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
                                  mavlink_message_t globalPositionIntMsg;

                                  uint8_t system_id = 0;
                                  uint8_t component_id = 0;
                                  int32_t time_boot_ms = 0;
                                  int32_t lat = 0;
                                  int32_t lon = 0;
                                  int32_t alt = 0;
                                  int32_t relative_alt = 0;
                                  int16_t vx = 0;
                                  int16_t vy = 0;
                                  int16_t vz = 0;
                                  uint16_t hdg = 0;

                                  mavlink_msg_global_position_int_pack(
                                      system_id, component_id, &globalPositionIntMsg, time_boot_ms,
                                      lat, lon, alt, relative_alt, vx, vy, vz, hdg);

                                  tm.MT.addMavlinkMsgToByteQueue(globalPositionIntMsg,
                                                                 GSC.highPriorityTransmitBuffer);
                                  // END: ingest drone state data and pack bytes into
                                  // GSC.highPriorityTransmitBuffer

                                  // transmit the data via GSC.transmit(); function
                                  GSC.transmit(GSC.highPriorityTransmitBuffer);

                                  // The interval at which the instructions in the lambda function
                                  // are executed.
                                  vTaskDelay(pdMS_TO_TICKS(500));  // Adjust the delay as necessary
                              }
                          });
}

void TelemetryManager::update() {
    /*
     * @brief the following code up to END is responsible for taking data from other managers and
     * converting them to Mavlink bytes, then putting them into GSC.lowPriorityTransmitBuffer.
     */

    // START: fill GSC.lowPriorityTransmitBuffer with data to transmit

    // END: fill GSC.lowPriorityTransmitBuffer with data to transmit

    /*
     * the following code up to END is responsible for taking the bytes from the
     * GSC.DMAReceiveBuffer and converting them to Mavlink messages/triggering the callbacks
     * associated with each Mavlink message.
     */
    // START: convert bytes from GSC.DMAReceiveBuffer to Mavlink messages
    MT.bytesToMavlinkMsg(GSC.DMAReceiveBuffer);
    // END: convert bytes from GSC.DMAReceiveBuffer to Mavlink messages

    // transmit non routine data via GSC.transmit(); function
    GSC.transmit(GSC.lowPriorityTransmitBuffer);
}

void TelemetryManager::teardownTasks() { delete routineDataTransmission; }
