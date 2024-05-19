// This file was created by Yarema Dzulynsky on 2023-07-24.
#include "mavlink_encoder.hpp"
#include <TMCircularBuffer.hpp>

// Default constructor for the MavlinkEncoder class.
MavlinkEncoder::MavlinkEncoder() = default;

void packIntoCircularBuffer(size_t bufferLen, TMCircularBuffer &circularBuffer,
                            const uint8_t *bufferToAdd) {
    // Copy the contents of bufferToAdd into circularBuffer.
    for (size_t i = 0; i < bufferLen; i++) {
        circularBuffer.enqueue(bufferToAdd[i]);
    }
}




void MavlinkEncoder::packStateData(mavlink_global_position_int_t& mavGPI, mavlink_attitude_t& mavAtt, TMCircularBuffer& transmissionBuffer) {
    
    // Initialize the buffer for the global position and attitude messages.
    uint8_t global_position_buffer[MAVLINK_MAX_PACKET_LEN] = {0};
    uint8_t attitude_buffer[MAVLINK_MAX_PACKET_LEN] = {0};

    // Initialize the buffer sizes for the global position and attitude messages.
    std::size_t global_position_buffer_size;
    std::size_t attitude_buffer_size;

    // Pack the mavlink global position message into global position buffer.
    global_position_buffer_size = ENCODE_MESSAGE(global_position_int, mavGPI.time_boot_ms, mavGPI.lat, mavGPI.lon, mavGPI.alt, mavGPI.relative_alt,
                                  mavGPI.vx, mavGPI.vy, mavGPI.vz, mavGPI.hdg)(currentMessage, global_position_buffer);

    // Pack the mavlink attitude message into attitude buffer.
    attitude_buffer_size = ENCODE_MESSAGE(attitude, mavAtt.time_boot_ms, mavAtt.roll, mavAtt.pitch, mavAtt.yaw, mavAtt.rollspeed, mavAtt.pitchspeed, mavAtt.yawspeed)(currentMessage, attitude_buffer);
    
    // Pack the global position and attitude messages into the transmission buffer.
    packIntoCircularBuffer(global_position_buffer_size, transmissionBuffer, global_position_buffer);
    packIntoCircularBuffer(attitude_buffer_size, transmissionBuffer, attitude_buffer);

}