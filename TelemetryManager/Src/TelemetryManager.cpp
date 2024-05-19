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

TelemetryManager::TelemetryManager() {
    this->MT = MavlinkTranslator();


    // Create the GSC object
    int length = 10 * MAVLINK_MAX_PACKET_LEN;
    uint8_t* lowPriorityTransmitBuffer = new uint8_t[length];
    uint8_t* highPriorityTransmitBuffer = new uint8_t[length];

    TMCircularBuffer& circularBufferRef = *static_cast<TMCircularBuffer*>(rfd900_circular_buffer);

    // Had to make this dynamic because it has a reference to a circular buffer. C++ doesn't allow
    // otherwise.
    this->GSC = new GroundStationCommunication(circularBufferRef, lowPriorityTransmitBuffer,
                                               highPriorityTransmitBuffer, length);
}

TelemetryManager::~TelemetryManager() {
    // Destructor
    teardownTasks();
    delete GSC;
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
    routineDataTransmission = new TelemetryTask(
        "routineDataTransmission", configMINIMAL_STACK_SIZE, tskIDLE_PRIORITY, *this,
        [](TelemetryManager& tm) -> void {
            auto GSC = tm.GSC;
            while (true) {
                // START: ingest drone state data and pack bytes into
                // GSC.highPriorityTransmitBuffer

                mavlink_global_position_int_t position;
                mavlink_attitude_t attitude;

                // Replace with actual data from the drone. This is just a placeholder.
                position.alt = -1;
                position.lat = -1;
                position.lon = -1;
                position.vx = -1;
                position.vy = -1;
                position.vz = -1;
                position.hdg = -1;
                position.relative_alt = -1;
                position.time_boot_ms = -1;

                attitude.roll = -1;
                attitude.pitch = -1;
                attitude.yaw = -1;
                attitude.rollspeed = -1;
                attitude.pitchspeed = -1;
                attitude.yawspeed = -1;
                attitude.time_boot_ms = -1;

                tm.MT.encoder.packStateData(position, attitude, GSC->highPriorityTransmitBuffer);

                // END: ingest drone state data and pack bytes into
                // GSC.highPriorityTransmitBuffer

                // transmit the data via GSC.transmit(); function
                GSC->transmit(GSC->highPriorityTransmitBuffer);

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
    MT.bytesToMavlinkMsg(GSC->DMAReceiveBuffer);
    // END: convert bytes from GSC.DMAReceiveBuffer to Mavlink messages

    // transmit non routine data via GSC.transmit(); function
    GSC->transmit(GSC->lowPriorityTransmitBuffer);
}

void TelemetryManager::teardownTasks() {
    delete routineDataTransmission;
}
