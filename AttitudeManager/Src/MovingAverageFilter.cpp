#include "MovingAverageFilter.hpp"
#include "FilteringGeneral.hpp"

#include <string.h>
#include <math.h>

float sumBuf(float* buf, int count) {
  float sum = 0;
  for (int i = 0; i < count; i++) {
    sum += buf[i];
  }
  return sum;
}

int MovingAverageFiltering::WindowCount = 0;

FilteringError_t MovingAverageFiltering::Init() {
    FilteringError_t ret;
    ret.errorCode = 0;

    memset(Window.magxVals, 0, sizeof(WINDOW_SIZE));

    return ret;
}

FilteringError_t MovingAverageFiltering::Update(MovingAverageOutput_t* output) {
    FilteringError_t ret;
    ret.errorCode = 0;

    // Get the updated IMU data.
    IMUData_t imuData;
    FilteringGeneral::imuObj->GetResult(imuData);
    if (!imuData.isDataNew) {
        ret.errorCode = 1;
        return ret;
    }

    // Update the window (acts as a circular buffer, we can later use circular_buffer.hpp if we make
    // the types generic for that).
    int overwritePos = WindowCount % WINDOW_SIZE;
    Window.magxVals[overwritePos] = imuData.magx;
    WindowCount++;

    // Recompute the average.
    unsigned int count = std::min(WindowCount, WINDOW_SIZE);
    if (count == 0) count = 1; // Avoid division by 0.

    float magxSum = sumBuf(Window.magxVals, count);
    output->magx = magxSum / count;

    return ret;
}