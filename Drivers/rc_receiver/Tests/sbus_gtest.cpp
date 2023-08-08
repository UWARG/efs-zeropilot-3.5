#include <gtest/gtest.h>
#include <fff.h>
#include "sbus_receiver.hpp"

DEFINE_FFF_GLOBALS;

/*
    Faking the HAL function in this unit test to do nothing
    The recieved data will be manually added to test cases
*/
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_Receive_DMA, UART_HandleTypeDef*, uint8_t*, uint16_t );

/*
    TEST1: Preliminary sanity check to the sbus parser
*/
TEST (SBUS_Receiver_Test, RawDataToSBus){
    const uint8_t in_bytes[SBUS_FRAME_SIZE] = {
        HEADER_, 0x80, 0x08, 0x84,
        0x20, 0x06, 0x41, 0x88, 0x42, 0x18,
        0xE2, 0x10, 0x88, 0x48, 0x84, 0x22,
        0x16, 0xC1, 0x88, 0x46, 0x38, 0xE2,
        0x11, 0x00, FOOTER_
    };

    SBus_t packet{
        packet.ch[0] = 128,
        packet.ch[1] = 129,
        packet.ch[2] = 130,
        packet.ch[3] = 131,
        packet.ch[4] = 132,
        packet.ch[5] = 133,
        packet.ch[6] = 134,
        packet.ch[7] = 135,
        packet.ch[8] = 136,
        packet.ch[9] = 137,
        packet.ch[10] = 138,
        packet.ch[11] = 139,
        packet.ch[12] = 140,
        packet.ch[13] = 141,
        packet.ch[14] = 142,
        packet.ch[15] = 143
    };

    UART_HandleTypeDef uart_fake;
    SBUSReceiver sbus_test(&uart_fake);
    memcpy(sbus_test.raw_sbus_, in_bytes, sizeof(in_bytes));
    sbus_test.parse();

    EXPECT_TRUE(sbus_test.GetSBUS().new_data);

    for(int i = 0; i < SBUS_INPUT_CHANNELS; i ++){
        EXPECT_EQ(sbus_test.GetSBUS().ch[i], packet.ch[i])
                    << "Error mapping happened at channel " << i <<  ".";
    }

    /*
        ControlSignal should all be zero here because the lowest sbus value 
        aka SBUS_RANGE_MIN is usually below 200
    */
    for(int i = 0; i < SBUS_INPUT_CHANNELS; i ++){
        EXPECT_EQ(sbus_test.GetRCControl().ControlSignals[i], 0)
                    << "Error mapping happened at channel " << i <<  ".";
    }
}

/*
    TEST2: A more strict sbus parser test
*/
TEST (SBUS_Receiver_Test, RawDataToSBusSecond){
    const uint8_t in_bytes[SBUS_FRAME_SIZE] = {
        HEADER_, 0xC0, 0x00, 0x38,
        0x30, 0x00, 0x0E, 0x3E, 0xF0, 0x01,
        0x03, 0x18, 0x00, 0x07, 0x78, 0xB5,
        0xAA, 0x05, 0x25, 0x28, 0x01, 0x08,
        0x40, 0x00, FOOTER_
    };

    SBus_t packet{
        packet.ch[0] = SBUS_RANGE_MIN,
        packet.ch[1] = SBUS_RANGE_MAX,
        packet.ch[2] = SBUS_RANGE_MIN,
        packet.ch[3] = SBUS_RANGE_MAX,
        packet.ch[4] = 992,
        packet.ch[5] = 992,
        packet.ch[6] = SBUS_RANGE_MIN,
        packet.ch[7] = SBUS_RANGE_MIN,
        packet.ch[8] = SBUS_RANGE_MAX,
        packet.ch[9] = SBUS_RANGE_MAX,
        packet.ch[10] = 725,
        packet.ch[11] = 725,
        packet.ch[12] = 592,
        packet.ch[13] = 592,
        packet.ch[14] = 512,
        packet.ch[15] = 512
    };

    RCControl expectedRC;
    {
        expectedRC.ControlSignals[0] = 0;
        expectedRC.ControlSignals[1] = 100;
        expectedRC.ControlSignals[2] = 0;
        expectedRC.ControlSignals[3] = 100;
        expectedRC.ControlSignals[4] = 50;
        expectedRC.ControlSignals[5] = 50;
        expectedRC.ControlSignals[6] = 0;
        expectedRC.ControlSignals[7] = 0;
        expectedRC.ControlSignals[8] = 100;
        expectedRC.ControlSignals[9] = 100;
        expectedRC.ControlSignals[10] = 33.3125;
        expectedRC.ControlSignals[11] = 33.3125;
        expectedRC.ControlSignals[12] = 25;
        expectedRC.ControlSignals[13] = 25;
        expectedRC.ControlSignals[14] = 20;
        expectedRC.ControlSignals[15] = 20;
    }

    UART_HandleTypeDef uart_fake;
    SBUSReceiver sbus_test(&uart_fake);
    memcpy(sbus_test.raw_sbus_, in_bytes, sizeof(in_bytes));
    sbus_test.parse();

    EXPECT_TRUE(sbus_test.GetSBUS().new_data);

    for(int i = 0; i < SBUS_INPUT_CHANNELS; i ++){
        EXPECT_EQ(sbus_test.GetSBUS().ch[i], packet.ch[i])
                    << "Error mapping happened at channel " << i <<  ".";
    }

    for(int i = 0; i < SBUS_INPUT_CHANNELS; i ++){
        EXPECT_EQ(sbus_test.GetRCControl().ControlSignals[i], expectedRC.ControlSignals[i])
                    << "Error mapping happened at channel " << i <<  ".";
    }
}

/*
    TEST3: Expect to prove the operator override and named array element to work
*/
TEST (RCControlStructTest, NamedArrayElement){
    RCControl expectedRC;
    {
        expectedRC.ControlSignals[0] = 0;
        expectedRC.ControlSignals[1] = 100;
        expectedRC.ControlSignals[2] = 0;
        expectedRC.ControlSignals[3] = 100;
        expectedRC.ControlSignals[4] = 50;
        expectedRC.ControlSignals[5] = 50;
        expectedRC.ControlSignals[6] = 0;
        expectedRC.ControlSignals[7] = 0;
        expectedRC.ControlSignals[8] = 100;
        expectedRC.ControlSignals[9] = 100;
        expectedRC.ControlSignals[10] = 100.0 / 3.0;
        expectedRC.ControlSignals[11] = 100.0 / 3.0;
        expectedRC.ControlSignals[12] = 25;
        expectedRC.ControlSignals[13] = 25;
        expectedRC.ControlSignals[14] = 20;
        expectedRC.ControlSignals[15] = 20;
    }

    EXPECT_TRUE(expectedRC[0] == expectedRC.roll);
    EXPECT_TRUE(expectedRC[1] == expectedRC.pitch);
    EXPECT_TRUE(expectedRC[2] == expectedRC.throttle);
    EXPECT_TRUE(expectedRC[3] == expectedRC.yaw);
    EXPECT_TRUE(expectedRC[4] == expectedRC.arm);
    EXPECT_TRUE(expectedRC[5] == expectedRC.aux1);
    EXPECT_TRUE(expectedRC[6] == expectedRC.aux2);
    EXPECT_TRUE(expectedRC[7] == expectedRC.aux3);
    EXPECT_TRUE(expectedRC[8] == expectedRC.aux4);
    EXPECT_TRUE(expectedRC[9] == expectedRC.aux5);
    EXPECT_TRUE(expectedRC[10] == expectedRC.aux6);
    EXPECT_TRUE(expectedRC[11] == expectedRC.aux7);
    EXPECT_TRUE(expectedRC[12] == expectedRC.aux8);
    EXPECT_TRUE(expectedRC[13] == expectedRC.aux9);
    EXPECT_TRUE(expectedRC[14] == expectedRC.aux10);
    EXPECT_TRUE(expectedRC[15] == expectedRC.aux11);  
}



