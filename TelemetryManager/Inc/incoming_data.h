//
// Created by Yarema Dzulynsky on 2023-08-30.
//

#ifndef ZEROPILOT_SW_3_5_3_INCOMINGDATA_H
#define ZEROPILOT_SW_3_5_3_INCOMINGDATA_H

/**
 * @brief Represents a structure for incoming navigation data.
 *
 * This structure is designed to hold various navigation parameters such as
 * latitude, longitude, velocities in various axes, altitude, and the pitch,
 * roll, and yaw angles. Additionally, for each data point, there is a
 * corresponding boolean flag to indicate whether that data point has been
 * initialized or not. This helps in ensuring data validity before processing.
 *
 * @note This structure should be modified to suit the needs of the project.
 *     This is just a preliminary structure to get us started.
 */
struct IncomingData {

    float latitude = 0;                ///< Latitude of the drone in degrees.
    bool isLatitudeInitialized = false;///< Flag indicating if latitude value has been set.

    float longitude = 0;               ///< Longitude of the drone in degrees.
    bool isLongitudeInitialized = false;///< Flag indicating if longitude value has been set.

    float vx = 0;                      ///< Velocity in the X-axis (m/s).
    bool isVxInitialized = false;      ///< Flag indicating if vx value has been set.

    float vy = 0;                      ///< Velocity in the Y-axis (m/s).
    bool isVyInitialized = false;      ///< Flag indicating if vy value has been set.

    float vz = 0;                      ///< Velocity in the Z-axis (m/s).
    bool isVzInitialized = false;      ///< Flag indicating if vz value has been set.

    int altitude = 0;                  ///< Altitude of the drone in meters.
    bool isAltitudeInitialized = false;///< Flag indicating if altitude value has been set.

    float pitch = 0;                   ///< Pitch angle of the drone in degrees.
    bool isPitchInitialized = false;   ///< Flag indicating if pitch value has been set.

    float roll = 0;                    ///< Roll angle of the drone in degrees.
    bool isRollInitialized = false;    ///< Flag indicating if roll value has been set.

    float yaw = 0;                     ///< Yaw angle of the drone in degrees.
    bool isYawInitialized = false;     ///< Flag indicating if yaw value has been set.
};

#endif  // ZEROPILOT_SW_3_5_3_INCOMINGDATA_H
