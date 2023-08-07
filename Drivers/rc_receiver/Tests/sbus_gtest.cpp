#include <gtest/gtest.h>
#include <fff.h>
#include "sbus_receiver.hpp"

DEFINE_FFF_GLOBALS;

/*
    Faking the HAL function in this unit test to do nothing
    The recieved data will be manually added to test cases
*/
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_Receive_DMA, UART_HandleTypeDef*, uint8_t*, uint16_t );

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

    for(int i = 0; i < SBUS_INPUT_CHANNELS; i ++){
        EXPECT_TRUE(sbus_test.GetSBUS().ch[i] == packet.ch[i]);
    }

    for(int i = 0; i < SBUS_INPUT_CHANNELS; i ++){
        EXPECT_TRUE(sbus_test.GetRCControl().ControlSignals[i] == 0);
    }
}

/*
    Expect to prove the operator override and named array element to work
*/
TEST (RCControlStructTest, NamedArrayElement){
    RCControl receivedRC;
    {
        receivedRC.ControlSignals[0] = 0;
        receivedRC.ControlSignals[1] = 100;
        receivedRC.ControlSignals[2] = 0;
        receivedRC.ControlSignals[3] = 100;
        receivedRC.ControlSignals[4] = 50;
        receivedRC.ControlSignals[5] = 50;
        receivedRC.ControlSignals[6] = 0;
        receivedRC.ControlSignals[7] = 0;
        receivedRC.ControlSignals[8] = 100;
        receivedRC.ControlSignals[9] = 100;
        receivedRC.ControlSignals[10] = 100.0 / 3.0;
        receivedRC.ControlSignals[11] = 100.0 / 3.0;
        receivedRC.ControlSignals[12] = 25;
        receivedRC.ControlSignals[13] = 25;
        receivedRC.ControlSignals[14] = 20;
        receivedRC.ControlSignals[15] = 20;
    }

    EXPECT_TRUE(receivedRC[0] == receivedRC.roll);
    EXPECT_TRUE(receivedRC[1] == receivedRC.pitch);
    EXPECT_TRUE(receivedRC[2] == receivedRC.throttle);
    EXPECT_TRUE(receivedRC[3] == receivedRC.yaw);
    EXPECT_TRUE(receivedRC[4] == receivedRC.arm);
    EXPECT_TRUE(receivedRC[5] == receivedRC.aux1);
    EXPECT_TRUE(receivedRC[6] == receivedRC.aux2);
    EXPECT_TRUE(receivedRC[7] == receivedRC.aux3);
    EXPECT_TRUE(receivedRC[8] == receivedRC.aux4);
    EXPECT_TRUE(receivedRC[9] == receivedRC.aux5);
    EXPECT_TRUE(receivedRC[10] == receivedRC.aux6);
    EXPECT_TRUE(receivedRC[11] == receivedRC.aux7);
    EXPECT_TRUE(receivedRC[12] == receivedRC.aux8);
    EXPECT_TRUE(receivedRC[13] == receivedRC.aux9);
    EXPECT_TRUE(receivedRC[14] == receivedRC.aux10);
    EXPECT_TRUE(receivedRC[15] == receivedRC.aux11);  
}



