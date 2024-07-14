/**
 * @file TelemetryManager.hpp
 * @brief What does this file do?
 *
 * @note Anything future maintainers should know about this file?
 *
 * @version 1.0
 * @date 2023-08-24
 * @author Yarema Dzulynsky: initial structure & Implementation
 *
 * @warning Any issues you think are important/foresee in the future?
 */

#ifndef TELEMETRYMANAGER_H
#define TELEMETRYMANAGER_H

#include "GroundStationCommunication.hpp"
#include "MavlinkTranslator.hpp"

class TelemetryManager {
   private:
    GroundStationCommunication GSC;
    MavlinkTranslator MT;
    TMCircularBuffer* DMAReceiveBuffer;
    uint8_t* lowPriorityTransmitBuffer;
    uint8_t* highPriorityTransmitBuffer;

    //A reference to the drone's altitude variable
    uint8_t& altitude;

    /**
     * @brief Create and configure FreeRTOS tasks.
     *
     */
    void spinUpTasks();

    /**
     * @brief Destroy FreeRTOS tasks and free up memory.
     *
     */
    void teardownTasks();

   public:
    /**
     * @brief Construct a new Telemetry Manager object. Does not initialize the tasks.
     * To do so call the init() method.
     * @param altitude A reference to the drone's altitude variable
     */
    TelemetryManager(uint8_t& altitude);

    /**
     * @brief Destroy the Telemetry Manager object. Also destroys the tasks.
     */
    ~TelemetryManager();

    /**
     * @brief Initialize TM.
     *
     */
    void init();

    /**
     * @brief Anything that is managed by SM should be updated here. This is basically the main loop
     * of TM.
     *
     *
     */
    void update();
};

#endif
