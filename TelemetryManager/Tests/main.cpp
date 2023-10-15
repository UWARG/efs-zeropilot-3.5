#include <iostream>
#include "../Inc/mavlink_decoder.h"
#include "../Inc/mavlink_encoder.h"
#include "../Inc/incoming_data.h"

// Create instances of MavlinkEncoder and MavlinkDecoder classes.
MavlinkEncoder mavlinkEncoder;
MavlinkDecoder mavlinkDecoder;

int main() {
    // Initialize a new IncomingData object with some example data values.
    IncomingData data;
    data.latitude = 1;
    data.longitude = 2;
    data.altitude = 3;
    data.isLatitudeInitialized = true;
    data.isLongitudeInitialized = true;
    data.isAltitudeInitialized = true;
    data.isRollInitialized = true;
    data.isPitchInitialized = true;
    data.isYawInitialized = true;
    data.roll = 4;
    data.pitch = 5;
    data.yaw = 6;

    std::cout << "Initialized incoming data with sample values." << std::endl;

    // Define a buffer to store the MAVLink messages.
    uint8_t buffer[MAX_ARRAY_BUFFER_SIZE];

    // Use the mavlinkEncoder to find the right packing function for the incoming data.
    // This will encode the data into MAVLink format and store it in the buffer.
    mavlinkEncoder.packIntoMavlinkByteArray(data, buffer, MAX_ARRAY_BUFFER_SIZE);

    std::cout << "The incoming data has been encoded into MAVLink messages and stored in a buffer." << std::endl;

    // At this point, you can do whatever you want with the buffer.
    // In this example, we will decode the MAVLink messages back into their original format.

    // Use the mavlinkDecoder to parse the buffer back into MAVLink messages.
    // This will use the callbacks specified in the MavlinkDecoder class to process the messages.
    mavlinkDecoder.parseBytesToMavlinkMsgs(buffer, MAX_ARRAY_BUFFER_SIZE);

    std::cout << "Parsed the buffer back into MAVLink messages using MavlinkDecoder." << std::endl;

    return 0;
}
