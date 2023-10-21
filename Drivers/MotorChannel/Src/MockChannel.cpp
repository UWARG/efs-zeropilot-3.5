#include "MockChannel.hpp"

namespace mock {
    uint8_t *motorOutputs = nullptr;
}

MockChannel::MockChannel(uint8_t channel):
                        channel_(channel)
{}

void MockChannel::set(uint8_t percent) {
    mock::motorOutputs[channel_] = percent;
}
