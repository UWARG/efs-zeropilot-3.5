#ifndef MOVING_AVERAGE_HPP
#define MOVING_AVERAGE_HPP

#include <stdint.h>

#define MAX_WINDOW_SIZE 50
#define MAX_POS_LIMIT MAX_WINDOW_SIZE * 1000000

enum MovingAverageError_t {
  SUCCESS,
  FAILURE,
  OLD_VALUES,
};

template<typename T>
class Window {
private:
  T window[MAX_WINDOW_SIZE];
  uint8_t size;
  uint32_t pos;
public:
  Window();
  void update(T newValue);
  MovingAverageError_t getAverage(uint8_t n, T* result);
};

#endif // MOVING_AVERAGE_HPP
