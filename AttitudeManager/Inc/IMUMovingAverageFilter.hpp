#ifndef IMU_MOVING_AVERAGE_FILTER_HPP
#define IMU_MOVING_AVERAGE_FILTER_HPP

#include "MovingAverage.hpp"

class IMUMovingAverageFilter {
public:
  IMUMovingAverageFilter();
  MovingAverageError_t Update();
  MovingAverageError_t GetAverage(uint8_t windowSize, IMUData_t* FilteredIMUData);

private:
  IMU* imuObj;
  Window<float> magxVals;
  Window<float> magyVals;
  Window<float> magzVals;
  Window<float> accxVals;
  Window<float> accyVals;
  Window<float> acczVals;
  Window<float> gyrxVals;
  Window<float> gyryVals;
  Window<float> gyrzVals;
};

#endif // IMU_MOVING_AVERAGE_FILTER_HPP
