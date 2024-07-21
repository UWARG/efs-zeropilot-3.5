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
// #include "DiagnosticLEDs.hpp"
#include "FreeRTOS.h"
#include "task.h"

class TelemetryManager {
   public:
    GroundStationCommunication GSC;
    MavlinkTranslator MT;
    TMCircularBuffer* DMAReceiveBuffer;
    uint8_t* lowPriorityTransmitBuffer;
    uint8_t* highPriorityTransmitBuffer;

    // A reference to the drone's altitude variable
    int32_t& alt;
    int32_t& lat;
    int32_t& lon;
    int32_t& relative_alt;
    int16_t& vx;
    int16_t& vy;
    int16_t& vz;
    int& hdg;
    int32_t& time_boot_ms;
    MAV_STATE& state;
    MAV_MODE_FLAG& mode;

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
    /**
     * @brief Construct a new Telemetry Manager object. Does not initialize the tasks.
     * To do so call the init() method.
     * @param altitude A reference to the drone's altitude variable
     */
    TelemetryManager(int32_t& lat, int32_t& lon, int32_t& alt, int32_t& relative_alt, int16_t& vx,
                     int16_t& vy, int16_t& vz, int& hdg, int32_t& time_boot_ms, MAV_STATE& state, MAV_MODE_FLAG& mode);

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
