#include "manual.hpp"
#include "CommonDataTypes.hpp"

namespace AM {

AttitudeManagerInput Manual::run(const AttitudeManagerInput& input) {
    return input;
}

void Manual::updatePid() {}

}  // namespace AM
