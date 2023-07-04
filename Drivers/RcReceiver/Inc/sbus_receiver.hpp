#ifndef SBUS_RECEIVER_HPP
#define SBUS_RECEIVER_HPP

#include "usart.h"
#include "rcreceiver_datatypes.h"
#include "sbus_defines.h"

/*
 * UART Config for this sbus driver
 * Word Length - 9 bits(Including Parity)
 * Parity - Even
 * Stop Bits - 2
 * TX&RX Pin Activitive Level Inversion - enabled (this can depend on the type of sbus)
 * enable dma interrupt for uart_rx
 * don't forget to enable uart IRQ
 */

class SBUSReceiver{
    public:
    	SBUSReceiver(UART_HandleTypeDef* uart);

    /*
        get the sbus data
        @return SBus struct
    */
        SBus_t GetSBUS();

    /*
        get the RCControl data that is parsed from sbus
        more readable data that varies from 0 to 100
        @return RCControl struct
    */
        RCControl GetRCControl();

        void parse();
    /*
        public variable
    */
        uint8_t raw_sbus_[SBUS_FRAME_SIZE];

    private:

    /*
        object constructor
        @param uart instance ie. &huart1
    */

    // member variables
        UART_HandleTypeDef* uart_;
        SBus_t received_sbus_;
        RCControl received_rccontrol_;

    //private functions
        void read();
        void cast_rccontrol();
        float sbus_to_rccontrol(uint16_t channel_value);

};


#endif