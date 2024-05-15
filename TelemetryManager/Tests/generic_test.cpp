#include <gtest/gtest.h>
// #include <iomanip>
#include "../Inc/incoming_data.h"
#include "../Inc/mavlink_decoder.h"
#include "../Inc/mavlink_encoder.h"

void fillTestData(IncomingData &data) {
    data.latitude = 1;
    data.longitude = 2;
    data.altitude = 3;
    data.isLatitudeInitialized = true;
    data.isLongitudeInitialized = true;
    data.isAltitudeInitialized = true;
    data.roll = 4;
    data.pitch = 5;
    data.yaw = 6;
    data.isRollInitialized = true;
    data.isPitchInitialized = true;
    data.isYawInitialized = true;
    data.vx = 7;
    data.vy = 8;
    data.vz = 9;
    data.isVxInitialized = true;
    data.isVyInitialized = true;
    data.isVzInitialized = true;
}



TEST(MavlinkEncoderTest, PackIntoMavlinkByteArrayTest) {
    MavlinkEncoder encoder;
    IncomingData data;

    // Fill data with test values
    fillTestData(data);

    uint8_t encoderBuffer[256] = {0};
    size_t encoderBufferSize = sizeof(encoderBuffer);
    encoder.packIntoMavlinkByteArray(data, encoderBuffer, encoderBufferSize);

    // Create mavlink messages
    mavlink_message_t globalPositionIntMsg, attitudeMsg;
    uint8_t globalPositionIntBuffer[MAVLINK_MAX_PACKET_LEN] = {0};
    uint8_t attitudeBuffer[MAVLINK_MAX_PACKET_LEN] = {0};

    mavlink_msg_global_position_int_pack(0, 0, &globalPositionIntMsg, 0, 1, 2, 3, 0, 7, 8, 9, 0);
    size_t globalPositionIntBufferLen = mavlink_msg_to_send_buffer(globalPositionIntBuffer, &globalPositionIntMsg);

    mavlink_msg_attitude_pack(0, 0, &attitudeMsg, 0, 4, 5, 6, 0, 0, 0);
    size_t attitudeBufferLen = mavlink_msg_to_send_buffer(attitudeBuffer, &attitudeMsg);

    uint8_t outputBuffer[MAVLINK_MAX_PACKET_LEN * 2] = {0};
    size_t outputIndex = 0;

    memcpy(outputBuffer, globalPositionIntBuffer, globalPositionIntBufferLen);
    outputIndex += globalPositionIntBufferLen;
    memcpy(outputBuffer + outputIndex, attitudeBuffer, attitudeBufferLen);
    outputIndex += attitudeBufferLen;

    int errorCount = 0;
    for (size_t i = 0; i < outputIndex; i++) {
        if (outputBuffer[i] != encoderBuffer[i]) {
            errorCount++;
        }
    }

    const int amountOfMessages = 2;
    ASSERT_TRUE(errorCount <= amountOfMessages * 3) << "There were " << errorCount << " errors in the encoder buffer";
}

TEST(MavlinkEncoderDecoderTest, EncodeThenDecodeTest) {
    MavlinkEncoder encoder;
    IncomingData data;

    // Fill data with test values
    fillTestData(data);

    uint8_t encoderBuffer[256] = {0};
    size_t encoderBufferSize = sizeof(encoderBuffer);
    encoder.packIntoMavlinkByteArray(data, encoderBuffer, encoderBufferSize);

    MavlinkDecoder decoder;
    int expectedNumberOfMessagesDecoded = 2;

    decoder.parseBytesToMavlinkMsgs(encoderBuffer, encoderBufferSize);
    ASSERT_EQ(MavlinkDecoder::decodedMessages, expectedNumberOfMessagesDecoded);

    MavlinkDecoder::decodedMessages = 0;
}

TEST(MavlinkDecoderTest, ParseBytesToMavlinkMsgsTest) {
    MavlinkDecoder decoder;
    int expectedNumberOfMessagesDecoded = 2;


        mavlink_message_t globalPositionIntMsg, attitudeMsg;
        uint8_t globalPositionIntBuffer[MAVLINK_MAX_PACKET_LEN] = {0};
        uint8_t attitudeBuffer[MAVLINK_MAX_PACKET_LEN] = {0};

        mavlink_msg_global_position_int_pack(0, 0, &globalPositionIntMsg, 0, 1, 2, 3, 0, 7, 8, 9, 0);
        size_t globalPositionIntBufferLen = mavlink_msg_to_send_buffer(globalPositionIntBuffer, &globalPositionIntMsg);

        mavlink_msg_attitude_pack(0, 0, &attitudeMsg, 0, 4, 5, 6, 0, 0, 0);
        size_t attitudeBufferLen = mavlink_msg_to_send_buffer(attitudeBuffer, &attitudeMsg);

        uint8_t outputBuffer[MAVLINK_MAX_PACKET_LEN * 2] = {0};

        memcpy(outputBuffer, globalPositionIntBuffer, globalPositionIntBufferLen);

        memcpy(outputBuffer + globalPositionIntBufferLen, attitudeBuffer, attitudeBufferLen);


    decoder.parseBytesToMavlinkMsgs(outputBuffer, globalPositionIntBufferLen + attitudeBufferLen);

    ASSERT_EQ(MavlinkDecoder::decodedMessages, expectedNumberOfMessagesDecoded);

    MavlinkDecoder::decodedMessages = 0;
}

TEST(EncoderMacroTest, global_position_int) {
    uint8_t buffer[256];

    // Use the ENCODE_MESSAGE macro to encode
    mavlink_message_t msg;
    ENCODE_MESSAGE(global_position_int, 1, 2, 3, 4, 5, 6, 7, 8, 9)(msg, buffer);

    // Decode the message
    mavlink_global_position_int_t globalPositionInt;
    mavlink_msg_global_position_int_decode(&msg, &globalPositionInt);

    // Assertions
    ASSERT_EQ(globalPositionInt.time_boot_ms, 1);
    ASSERT_EQ(globalPositionInt.lat, 2);
    ASSERT_EQ(globalPositionInt.lon, 3);
    ASSERT_EQ(globalPositionInt.alt, 4);
    ASSERT_EQ(globalPositionInt.relative_alt, 5);
    ASSERT_EQ(globalPositionInt.vx, 6);
    ASSERT_EQ(globalPositionInt.vy, 7);
    ASSERT_EQ(globalPositionInt.vz, 8);
    ASSERT_EQ(globalPositionInt.hdg, 9);
}