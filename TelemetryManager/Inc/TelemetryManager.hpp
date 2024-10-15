/**
 * @file TelemetryManager.hpp
 * @brief Manages multiple subclasses and FreeRTOS tasks that handle a 2 way communication link
 * between the drone and the ground station and encoding and decoding Mavlink messages. This class
 * essentially handles the link between the drone, other managers and Mission Planner.
 *
 * @note Anything future maintainers should know about this file?
 *
 * @version Milestone 2
 * @date 2023-08-24
 * @author Yarema Dzulynsky: initial structure & Implementation
 *
 * @warning Any issues you think are important/foresee in the future?
 */

#ifndef TELEMETRYMANAGER_H
#define TELEMETRYMANAGER_H

#include "FreeRTOS.h"
#include "GroundStationCommunication.hpp"
#include "MavlinkTranslator.hpp"
#include "task.h"

struct TMStateData {
    /*References to variables that contain the state of the drone (lat, lng, yaw, pitch, etc..).
     * They are updated by System Manager*/
    // Altitude (MSL) (unit:
    // mm)(https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    int32_t* alt;

    // The latitude of the drone (unit:
    // degE7)(https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    int32_t* lat;

    // The longitude of the drone (unit:
    // degE7)(https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    int32_t* lon;

    // Altitude above home (unit:
    // mm)(https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    int32_t* relative_alt;

    // Ground X Speed (Latitude, positive north) (unit:
    // cm/s)(https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    int16_t* vx;

    // Ground Y Speed (Longitude, positive east) (unit:
    // cm/s)(https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    int16_t* vy;

    // Ground Z Speed (Altitude, positive down) (unit:
    // cm/s)(https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    int16_t* vz;

    // Vehicle heading (yaw angle), 0.0..359.99 degrees. If unknown, set to: UINT16_MAX
    // (unit: cdeg)(https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    uint16_t* hdg;
    // Timestamp (time since system boot) (unit: ms)
    // (https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    int32_t* time_boot_ms;

    // Roll angle (-pi..+pi) (unit: rad)
    // (https://mavlink.io/en/messages/common.html#ATTITUDE).
    float* roll;

    // Pitch angle (-pi..+pi) (unit: rad)
    // (https://mavlink.io/en/messages/common.html#ATTITUDE).
    float* pitch;

    // Yaw angle (-pi..+pi) (unit: rad)
    // (https://mavlink.io/en/messages/common.html#ATTITUDE).
    float* yaw;

    // Roll angular speed (unit: rad/s)
    // (https://mavlink.io/en/messages/common.html#ATTITUDE).
    float* rollspeed;

    // Pitch angular speed (unit: rad/s)
    // (https://mavlink.io/en/messages/common.html#ATTITUDE).
    float* pitchspeed;

    // yawspeed Yaw angular speed (unit: rad/s)
    // (https://mavlink.io/en/messages/common.html#ATTITUDE).
    float* yawspeed;

};

class TelemetryManager {
   public:

    // struct containing variables that relate to the state of the drone (lat, lng, yaw, pitch, etc..).
    TMStateData stateData;

    // the buffer that stores the bytes received from the ground station.
    MavlinkTranslator& MT;

    // The object that facilitates communication with the ground station
    GroundStationCommunication& GSC;


    // System status flag (https://mavlink.io/en/messages/minimal.html#MAV_STATE).
    MAV_STATE& mavState;
    // System mode bitmap (https://mavlink.io/en/messages/minimal.html#MAV_MODE_FLAG).
    MAV_MODE_FLAG& mavMode;
    

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
     * @param stateData The state of the drone (lat, lng, yaw, pitch, etc..).
     * @param mav_state System status flag (https://mavlink.io/en/messages/minimal.html#MAV_STATE).
     * @param mav_mode System mode bitmap (https://mavlink.io/en/messages/minimal.html#MAV_MODE_FLAG).
     * @param GSC Object to handle communication with the groundstation
     * @param MT Object to translate MAVLink data
     */
    TelemetryManager(TMStateData& stateData, MAV_STATE& mavState, MAV_MODE_FLAG& mavMode, 
                     GroundStationCommunication& GSC, MavlinkTranslator& MT);

    /**
     * @brief Destroy the Telemetry Manager object. Also destroys the FreeRTOS tasks associated with
     * TM.
     */
    ~TelemetryManager();

    /**
     * @brief Initialize TM.
     *
     */
    void init();

    /**
     * @brief Anything that is managed by SM should be updated here. This is basically the main loop
     * of TM. Currently, it decodes MavLink messages from Missions Planner and packages and
     * transmits non-routine and routine data to the ground station (like battery voltage etc).
     */
    void update();
};

#endif
