#include "MovingAverage.hpp"
#include <string.h>

template<typename T>
Window<T>::Window() {
  memset(window, 0, MAX_WINDOW_SIZE);
  size = 0;
  pos = 0;
}

template<typename T>
void Window<T>::update(T newValue) {
  window[pos] = newValue;
  pos++;

  // If the window is not full yet, increase size.
  if (size < MAX_WINDOW_SIZE) {
    size++;
  }

  // Reset pos once in a while to ensure it doesn't get too big.
  if (pos == MAX_POS_LIMIT) {
    pos = 0;
  }
}

template<typename T>
MovingAverageError_t Window<T>::getAverage(uint8_t n, T* result) {
  if (n <= 0 || n > size) {
    return FAILURE;
  }

  int sum = 0;
  for (int i = pos; i > pos - n; i--) {
    uint8_t realIdx = pos % MAX_WINDOW_SIZE;
    sum += window[realIdx];
  }

  *result = static_cast<T>(sum) / n;
  return SUCCESS;
}
