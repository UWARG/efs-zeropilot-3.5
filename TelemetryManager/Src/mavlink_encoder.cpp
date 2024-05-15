// This file was created by Yarema Dzulynsky on 2023-07-24.
#include "../Inc/mavlink_encoder.h"

// Default constructor for the MavlinkEncoder class.
MavlinkEncoder::MavlinkEncoder() = default;

/**
 * Helper function to pack contents of one buffer into another array.
 *
 * @param bufferLen Length of buffer to add.
 * @param arrayBuffer Target buffer to which data will be added.
 * @param arrayIndex Current position/index in the target buffer where data should be added.
 * @param bufferToAdd Source buffer containing the data to be packed into the arrayBuffer.
 */
void packIntoArray(size_t bufferLen, uint8_t arrayBuffer[], size_t &arrayIndex,
                   const uint8_t *bufferToAdd) {
    // Copy the contents of bufferToAdd into arrayBuffer, starting at arrayIndex.
    for (size_t i = 0; i < bufferLen; i++) {
        arrayBuffer[arrayIndex++] = bufferToAdd[i];
    }
}

size_t MavlinkEncoder::packIntoMavlinkByteArray(IncomingData &data, uint8_t outputBuffer[], size_t maxBufferSize) {
    // Temporary buffer to hold MAVLink message before copying it to the output buffer.
    uint8_t tempBuffer[MAVLINK_MAX_PACKET_LEN] = {0};

    // Position/index tracker for the output buffer.
    size_t outputIndex = 0;
    std::size_t tempSize;

    // Check if any of the global position fields are initialized.
    if (data.isLatitudeInitialized || data.isLongitudeInitialized || data.isAltitudeInitialized) {
        // Encode the global_position_int message and store its size in tempSize.
        tempSize = ENCODE_MESSAGE(global_position_int, 0, data.latitude, data.longitude, data.altitude, 0,
                                  data.vx, data.vy, data.vz, 0)(currentMessage, tempBuffer);

        // Check if adding the message will overflow the outputBuffer.
        if(outputIndex + tempSize > maxBufferSize) {
            // Handle overflow (this could be extended to return an error, truncate, etc.)
        }

        // Copy the encoded message from tempBuffer to outputBuffer.
        packIntoArray(tempSize, outputBuffer, outputIndex, tempBuffer);
    }

    // Check if any of the attitude fields are initialized.
    if (data.isRollInitialized || data.isPitchInitialized || data.isYawInitialized) {
        // Encode the attitude message and store its size in tempSize.
        tempSize = ENCODE_MESSAGE(attitude, 0, data.roll, data.pitch, data.yaw, 0, 0, 0)(
            currentMessage, tempBuffer);

        // Check if adding the message will overflow the outputBuffer.
        if(outputIndex + tempSize > maxBufferSize) {
            // Handle overflow (this could be extended to return an error, truncate, etc.)
        }

        // Copy the encoded message from tempBuffer to outputBuffer.
        packIntoArray(tempSize, outputBuffer, outputIndex, tempBuffer);
    }

    // Return the actual size of the data that was packed into the outputBuffer.
    return outputIndex;
}