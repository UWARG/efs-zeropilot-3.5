#ifndef ZPSW3_AM_FBWA_HPP
#define ZPSW3_AM_FBWA_HPP

#include "flightmode.hpp"

namespace AM {

class FBWA : public Flightmode {
   public:
    FBWA() = default;

    AttitudeManagerInput run(const AttitudeManagerInput& input) override;
    void updatePid() override;
    void updatePidGains();
};

}  // namespace AM

#endif  // ZPSW3_AM_FBWA_HPP