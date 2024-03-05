#ifndef MOVING_AVERAGE_FILTER_HPP
#define MOVING_AVERAGE_FILTER_HPP

#define WINDOW_SIZE 50

#include "FilteringGeneral.hpp"
#include "circular_buffer.hpp"

struct MovingAverageOutput_t {
  float magx;
};

struct MovingAverageWindow {
  float magxVals[WINDOW_SIZE];
};

class MovingAverageFiltering {
public:
  static FilteringError_t Init();
  static FilteringError_t Update(MovingAverageOutput_t* output);

private:
  static MovingAverageWindow Window;
  static int WindowCount;
};

#endif
