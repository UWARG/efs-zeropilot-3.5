#include <iostream>

#include "../Inc/BasicMavlinkEncoder.h"
#include "../Inc/MavlinkDecoder.h"

auto *mavlinkEncoder = new BasicMavlinkEncoder();
auto *mavlinkDecoder = new MavlinkDecoder();

int main() {

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

    std::vector<uint8_t> buffer = mavlinkEncoder->findPackingFunction(data);

    //the message has now been parsed into multiple mavlink messages and stored in a buffer
    //do whatever you would like with that buffer


    //for example, you can parse the buffer back into mavlink messages, and use the callbacks specified in the
    // MavlinkDdecoder class to do whatever you would like with the messages.
    mavlinkDecoder->parseBytesToMavlinkMsgs(buffer.data(), buffer.size());

    return 0;
}
