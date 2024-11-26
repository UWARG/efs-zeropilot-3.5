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

#define CREATE_GETTER(type, member) \
    type get_##member() const { \
        return (member != nullptr) ? *member : std::numeric_limits<type>::max(); \
    }

#define CREATE_SETTER(type, member) \
    void set_##member(type* value) { member = value; }

#include "FreeRTOS.h"
#include "GroundStationCommunication.hpp"
#include "MavlinkTranslator.hpp"
#include "task.h"
#include <limits>

struct TMStateData {
   private:
    /*References to variables that contain the state of the drone (lat, lng, yaw, pitch, etc..).
     * They are updated by System Manager*/
    // Altitude (MSL) (unit:
    // mm)(https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    int32_t* alt = nullptr;

    // The latitude of the drone (unit:
    // degE7)(https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    int32_t* lat = nullptr;

    // The longitude of the drone (unit:
    // degE7)(https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    int32_t* lon = nullptr;

    // Altitude above home (unit:
    // mm)(https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    int32_t* relative_alt = nullptr;

    // Ground X Speed (Latitude, positive north) (unit:
    // cm/s)(https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    int16_t* vx = nullptr;

    // Ground Y Speed (Longitude, positive east) (unit:
    // cm/s)(https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    int16_t* vy = nullptr;

    // Ground Z Speed (Altitude, positive down) (unit:
    // cm/s)(https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    int16_t* vz = nullptr;

    // Vehicle heading (yaw angle), 0.0..359.99 degrees. If unknown, set to: UINT16_MAX
    // (unit: cdeg)(https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    uint16_t* hdg = nullptr;
    // Timestamp (time since system boot) (unit: ms)
    // (https://mavlink.io/en/messages/common.html#GLOBAL_POSITION_INT).
    int32_t* time_boot_ms = nullptr;

    // Roll angle (-pi..+pi) (unit: rad)
    // (https://mavlink.io/en/messages/common.html#ATTITUDE).
    float* roll = nullptr;

    // Pitch angle (-pi..+pi) (unit: rad)
    // (https://mavlink.io/en/messages/common.html#ATTITUDE).
    float* pitch = nullptr;

    // Yaw angle (-pi..+pi) (unit: rad)
    // (https://mavlink.io/en/messages/common.html#ATTITUDE).
    float* yaw = nullptr;

    // Roll angular speed (unit: rad/s)
    // (https://mavlink.io/en/messages/common.html#ATTITUDE).
    float* rollspeed = nullptr;

    // Pitch angular speed (unit: rad/s)
    // (https://mavlink.io/en/messages/common.html#ATTITUDE).
    float* pitchspeed = nullptr;

    // yawspeed Yaw angular speed (unit: rad/s)
    // (https://mavlink.io/en/messages/common.html#ATTITUDE).
    float* yawspeed = nullptr;

   public:
    // Generate getters using the macro
    CREATE_GETTER(int32_t, alt)
    CREATE_GETTER(int32_t, lat)
    CREATE_GETTER(int32_t, lon)
    CREATE_GETTER(int32_t, relative_alt)
    CREATE_GETTER(int16_t, vx)
    CREATE_GETTER(int16_t, vy)
    CREATE_GETTER(int16_t, vz)
    CREATE_GETTER(uint16_t, hdg)
    CREATE_GETTER(int32_t, time_boot_ms)
    CREATE_GETTER(float, roll)
    CREATE_GETTER(float, pitch)
    CREATE_GETTER(float, yaw)
    CREATE_GETTER(float, rollspeed)
    CREATE_GETTER(float, pitchspeed)
    CREATE_GETTER(float, yawspeed)

    CREATE_SETTER(int32_t, alt)
    CREATE_SETTER(int32_t, lat)
    CREATE_SETTER(int32_t, lon)
    CREATE_SETTER(int32_t, relative_alt)
    CREATE_SETTER(int16_t, vx)
    CREATE_SETTER(int16_t, vy)
    CREATE_SETTER(int16_t, vz)
    CREATE_SETTER(uint16_t, hdg)
    CREATE_SETTER(int32_t, time_boot_ms)
    CREATE_SETTER(float, roll)
    CREATE_SETTER(float, pitch)
    CREATE_SETTER(float, yaw)
    CREATE_SETTER(float, rollspeed)
    CREATE_SETTER(float, pitchspeed)
    CREATE_SETTER(float, yawspeed)
};

class TelemetryManager {
   public:
    // struct containing variables that relate to the state of the drone (lat, lng, yaw, pitch,
    // etc..).
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
     * @param mav_mode System mode bitmap
     * (https://mavlink.io/en/messages/minimal.html#MAV_MODE_FLAG).
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
