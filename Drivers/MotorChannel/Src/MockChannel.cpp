#include "MockChannel.hpp"

MockChannel::MockChannel():
                        percent_(0)
{}

void MockChannel::set(uint8_t percent) {
    percent_ = percent;
}
