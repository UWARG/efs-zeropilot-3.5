
#include "TelemetryManager.hpp"

// FreeRTOS task handle for the routineDataTransmission task
TaskHandle_t routineDataTransmissionH = NULL;

TelemetryManager::TelemetryManager(int32_t& lat, int32_t& lon, int32_t& alt, int32_t& relative_alt,
                                   int16_t& vx, int16_t& vy, int16_t& vz, uint16_t& hdg,
                                   int32_t& time_boot_ms, MAV_STATE& state, MAV_MODE_FLAG& mode,
                                   float& roll, float& pitch, float& yaw, float& rollspeed,
                                   float& pitchspeed, float& yawspeed)
    : DMAReceiveBuffer(new TMCircularBuffer(rfd900_circular_buffer)),
      lowPriorityTransmitBuffer(new uint8_t[RFD900_BUF_SIZE]),
      highPriorityTransmitBuffer(new uint8_t[RFD900_BUF_SIZE]),
      GSC(*DMAReceiveBuffer, lowPriorityTransmitBuffer, highPriorityTransmitBuffer,
          RFD900_BUF_SIZE),
      lat(lat),
      lon(lon),
      alt(alt),
      relative_alt(relative_alt),
      vx(vx),
      vy(vy),
      vz(vz),
      hdg(hdg),
      time_boot_ms(time_boot_ms),
      state(state),
      mode(mode),
      roll(roll),
      pitch(pitch),
      yaw(yaw),
      rollspeed(rollspeed),
      pitchspeed(pitchspeed),
      yawspeed(yawspeed) {}

TelemetryManager::~TelemetryManager() {
    // Destructor
    teardownTasks();
}

/**
 * @brief Initialize TM tasks (and other later).
 *
 */
void TelemetryManager::init() {
    // register TM tasks with FreeRTOS
    spinUpTasks();
}

/**
 * @brief This FreeRTOS task executes the code in the while loop every 500ms. It is responsible for
 * sending the highest priority/routine drone "state" data to the ground station. Data such as
 * heartbeat message, altitude, attitude, latitude, longitude... And anything else deemed
 * important enough to be transmitted at a regular interval. This is the highest priority
 * data in the GSC.highPriorityTransmitBuffer.
 */
void routineDataTransmission(void* pvParameters) {
    // placeholder for now
    uint8_t system_id = 0;
    // placeholder for now
    uint8_t component_id = 0;

    // for diagnostic purposes
    bool greenOn = false;
    bool blueOn = false;
    bool redOn = false;

    /*
    Cast the void pointer to a TelemetryManager pointer so we
    can access the TelemetryManager object's members.
    */
    auto* tm = static_cast<TelemetryManager*>(pvParameters);

    while (1) {
        /* For diagnostic purposes, toggle the green and blue LEDs on the Nucleo board
        HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin,
                          greenOn ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, blueOn ? GPIO_PIN_RESET : GPIO_PIN_SET);

        greenOn = !greenOn;
        blueOn = !blueOn;
        */

        // START: ingest drone state data and pack bytes into GSC.highPriorityTransmitBuffer

        /* Create a global_position_int message(lat lon alt relative_alt vx vy vz hdg - general
         * spatial data)*/
        mavlink_message_t globalPositionIntMsg = {0};

        // Pack the message with the actual data
        mavlink_msg_global_position_int_pack(system_id, component_id, &globalPositionIntMsg,
                                             tm->time_boot_ms, tm->lat, tm->lon, tm->alt,
                                             tm->relative_alt, tm->vx, tm->vy, tm->vz, tm->hdg);

        // Add the packed message to the byte queue for later transmission
        tm->MT.addMavlinkMsgToByteQueue(globalPositionIntMsg, tm->GSC.highPriorityTransmitBuffer);

        // Create an attitude message
        mavlink_message_t attitudeMsg = {0};
        // Pack the message with the actual data
        mavlink_msg_attitude_pack(system_id, component_id, &attitudeMsg, tm->time_boot_ms, tm->roll,
                                  tm->pitch, tm->yaw, tm->rollspeed, tm->pitchspeed, tm->yawspeed);
        // Add the packed message to the byte queue for later transmission
        tm->MT.addMavlinkMsgToByteQueue(attitudeMsg, tm->GSC.highPriorityTransmitBuffer);

        /* END: ingest drone state data and pack bytes into GSC.highPriorityTransmitBuffer*/

        // Create a heartbeat message
        mavlink_message_t heartbeatMsg = {0};
        // Pack the message with the actual data
        mavlink_msg_heartbeat_pack(system_id, component_id, &heartbeatMsg, MAV_TYPE_QUADROTOR,
                                   MAV_AUTOPILOT_INVALID, tm->mode, 0, tm->state);
        // Add the packed message to the byte queue for later transmission
        tm->MT.addMavlinkMsgToByteQueue(heartbeatMsg, tm->GSC.highPriorityTransmitBuffer);

        /*NOTE: The TelemetryManager object's members such as lat, lon, alt, relative_alt, vx, vy,
           vz etc are references to variables that are updated by SystemManager. Which is how they
           stay up to date
           */

        // transmit the high priority data to the ground station
        tm->GSC.transmit(tm->GSC.highPriorityTransmitBuffer);

        // The delay between each state data packing and transmission
        vTaskDelay(pdMS_TO_TICKS(500));  // Adjust the delay as necessary
    }
}
void TelemetryManager::spinUpTasks() {
    // Create a task that will send state data to the ground station every 500ms.
    xTaskCreate(&routineDataTransmission, "routineDataTransmission", 512UL, this, 24,
                &routineDataTransmissionH);
}

void TelemetryManager::update() {
    /*
     * @brief the following code up to END is responsible for taking data from other managers
     * and converting them to Mavlink bytes, then putting them into
     * GSC.lowPriorityTransmitBuffer.
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

void TelemetryManager::teardownTasks() {
    if (eTaskGetState(routineDataTransmissionH) != eDeleted) {
        vTaskDelete(routineDataTransmissionH);
    }
}
