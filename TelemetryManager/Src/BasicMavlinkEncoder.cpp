// Created by Yarema Dzulynsky on 2023-07-24.

#include "../Inc/BasicMavlinkEncoder.h"

BasicMavlinkEncoder::BasicMavlinkEncoder() = default;

/**
 * Add the contents of a buffer to the end of a vector
 * @param bufferLen - the length of the buffer
 * @param vectorBuffer - the vector to add the buffer to
 * @param buffer - the buffer to add to the vector
 */
void packIntoVector(size_t bufferLen, std::vector<uint8_t> &vectorBuffer, const uint8_t *bufferToAdd) {

    // Add the contents of the buffer to the vector with a for loop
    for (int i = 0; i < bufferLen; i++) {
        vectorBuffer.push_back(bufferToAdd[i]);
    }
}


std::vector<uint8_t> BasicMavlinkEncoder::findPackingFunction(IncomingData &data) {

    std::vector<uint8_t> vectorBuffer;
    std::size_t tempSize;

    auto *tempBuffer = new uint8_t[MAVLINK_MAX_PACKET_LEN];

    if(data.isLatitudeInitialized || data.isLongitudeInitialized || data.isAltitudeInitialized) {
        tempSize = ENCODE_MESSAGE(global_position_int, 0, data.latitude, data.longitude, data.altitude, 0, data.vx, data.vy, data.vz, 0)(currentMessage, tempBuffer);
        packIntoVector(tempSize, vectorBuffer, tempBuffer);
    }
    if(data.isRollInitialized || data.isPitchInitialized || data.isYawInitialized) {
        tempSize = ENCODE_MESSAGE(attitude, 0, data.roll, data.pitch, data.yaw, 0, 0, 0)(currentMessage, tempBuffer);
        packIntoVector(tempSize, vectorBuffer, tempBuffer);
    }

    delete[] tempBuffer;

    return vectorBuffer;
}

