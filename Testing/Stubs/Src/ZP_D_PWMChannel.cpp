#include "ZP_D_PWMChannel.hpp"

PWMChannel::PWMChannel(TIM_HandleTypeDef* timer,
                        uint16_t timerChannel)
{
    timerChannel_ = timerChannel;
    percent_ = 0;
}

void PWMChannel::set(uint8_t percent) {
    percent_ = percent;
}
