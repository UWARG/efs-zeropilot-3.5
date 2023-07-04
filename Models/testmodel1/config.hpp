#include "global_config.hpp"

extern Motor_t motors[];
extern const uint8_t NUM_MOTORS;

//TODO: Combine flightmode classes with AM control algorithms
class TempFlightmode1 : public Flightmode{};
class TempFlightmode2 : public Flightmode{};