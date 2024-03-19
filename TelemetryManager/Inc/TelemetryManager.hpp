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
#include "TelemetryTask.hpp"

class TelemetryManager {

    public:
    /**
     * @brief Construct a new Telemetry Manager object. Does not initialize the tasks. 
     * To do so call the init() method.
     */
    TelemetryManager();
    ~TelemetryManager();

    GroundStationCommunication GSC;
    MavlinkTranslator MT;

    /**
     * @brief Initialize TM.
     *
     */
    void init();

    /**
     * @brief This method is responsible for
     * sending non routine data to the ground station. Such as arm disarmed message status,
     * fulfilling data requests from the ground station etc. This is the lowest priority data
     * in the GSC.lowPriorityTransmitBuffer.
     */
    void transmitNonRoutineData();

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
};

#endif
