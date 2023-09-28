#include "sbus_receiver.hpp"
#include "drivers_config.hpp"

SBUSReceiver::SBUSReceiver(UART_HandleTypeDef* uart) : uart_(uart)
{
    for(int i = 0; i < SBUS_INPUT_CHANNELS; i++)
    {
        received_sbus_.ch[i] = 1000;
    }
    received_sbus_.ch17 = false;
    received_sbus_.ch18 = false;
    received_sbus_.failsafe = false;
    received_sbus_.lost_frame = false;
    received_sbus_.isDataNew = false;
    is_data_new_ = false;
    HAL_UART_Receive_DMA (uart_, raw_sbus_, SBUS_FRAME_SIZE);
}

SBus_t SBUSReceiver::GetSBUS(){

	/*volatile int counter;
	counter++;*/

	// if(uart_->RxState != HAL_UART_STATE_BUSY_RX) {
		/*counter++;*/
		HAL_UART_Receive_DMA (uart_, raw_sbus_, SBUS_FRAME_SIZE);

	// }


	received_sbus_.isDataNew = is_data_new_;
	is_data_new_ = false;
    return received_sbus_;
}

RCControl SBUSReceiver::GetRCControl(){

    if(uart_->RxState != HAL_UART_STATE_BUSY_RX) {
    	 HAL_UART_Receive_DMA (uart_, raw_sbus_, SBUS_FRAME_SIZE);
    }
    else {
    	cast_rccontrol();
    	received_rccontrol_.isDataNew = is_data_new_;
    	is_data_new_ = false;
    }

    return received_rccontrol_;
}

void SBUSReceiver::parse()
{
    if ((raw_sbus_[0] == HEADER_) && (raw_sbus_[24] == FOOTER_)) {

        //exactal parsing
        received_sbus_.ch[0]  = static_cast<int16_t>(raw_sbus_[1] |
                                            ((raw_sbus_[2] << 8) & 0x07FF));
        received_sbus_.ch[1]  = static_cast<int16_t>((raw_sbus_[2] >> 3) |
                                            ((raw_sbus_[3] << 5) & 0x07FF));
        received_sbus_.ch[2]  = static_cast<int16_t>((raw_sbus_[3] >> 6) |
                                            (raw_sbus_[4] << 2) |
                                            ((raw_sbus_[5] << 10) & 0x07FF));
        received_sbus_.ch[3]  = static_cast<int16_t>((raw_sbus_[5] >> 1) |
                                            ((raw_sbus_[6] << 7) & 0x07FF));
        received_sbus_.ch[4]  = static_cast<int16_t>((raw_sbus_[6] >> 4) |
                                            ((raw_sbus_[7] << 4) & 0x07FF));
        received_sbus_.ch[5]  = static_cast<int16_t>((raw_sbus_[7] >> 7) |
                                            (raw_sbus_[8] << 1) |
                                            ((raw_sbus_[9] << 9) & 0x07FF));
        received_sbus_.ch[6]  = static_cast<int16_t>((raw_sbus_[9] >> 2) |
                                            ((raw_sbus_[10] << 6) & 0x07FF));
        received_sbus_.ch[7]  = static_cast<int16_t>((raw_sbus_[10] >> 5) |
                                            ((raw_sbus_[11] << 3) & 0x07FF));
        received_sbus_.ch[8]  = static_cast<int16_t>(raw_sbus_[12] |
                                            ((raw_sbus_[13] << 8) & 0x07FF));
        received_sbus_.ch[9]  = static_cast<int16_t>((raw_sbus_[13] >> 3) |
                                            ((raw_sbus_[14] << 5) & 0x07FF));
        received_sbus_.ch[10] = static_cast<int16_t>((raw_sbus_[14] >> 6) |
                                            (raw_sbus_[15] << 2) |
                                            ((raw_sbus_[16] << 10) & 0x07FF));
        received_sbus_.ch[11] = static_cast<int16_t>((raw_sbus_[16] >> 1) |
                                            ((raw_sbus_[17] << 7) & 0x07FF));
        received_sbus_.ch[12] = static_cast<int16_t>((raw_sbus_[17] >> 4) |
                                            ((raw_sbus_[18] << 4) & 0x07FF));
        received_sbus_.ch[13] = static_cast<int16_t>((raw_sbus_[18] >> 7) |
                                            (raw_sbus_[19] << 1) |
                                            ((raw_sbus_[20] << 9) & 0x07FF));
        received_sbus_.ch[14] = static_cast<int16_t>((raw_sbus_[20] >> 2) |
                                            ((raw_sbus_[21] << 6) & 0x07FF));
        received_sbus_.ch[15] = static_cast<int16_t>((raw_sbus_[21] >> 5) |
                                            ((raw_sbus_[22] << 3) & 0x07FF));

        received_sbus_.ch17 = raw_sbus_[23] & CH17_MASK_;
        received_sbus_.ch18 = raw_sbus_[23] & CH18_MASK_;
        received_sbus_.lost_frame = raw_sbus_[23] & LOST_FRAME_MASK_;
        received_sbus_.failsafe = raw_sbus_[23] & FAILSAFE_MASK_;

        received_sbus_.isDataNew = true;

    }else{
    	received_sbus_.isDataNew = false;
    }
}

void SBUSReceiver::cast_rccontrol()
{
    for(uint8_t i = 0; i < 16; i++){
        received_rccontrol_.ControlSignals[i] = sbus_to_rccontrol(received_sbus_.ch[i]);
    }
}

 float SBUSReceiver::sbus_to_rccontrol(uint16_t channel_value)
 {
    if(channel_value < SBUS_RANGE_MIN)
        channel_value = SBUS_RANGE_MIN;
    if(channel_value > SBUS_RANGE_MAX)
        channel_value = SBUS_RANGE_MAX;
    return static_cast<float>((channel_value - SBUS_RANGE_MIN) * (100.0f / SBUS_RANGE_RANGE));
 }
