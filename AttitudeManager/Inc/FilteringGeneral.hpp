#ifndef FILTERINGGENERAL_HPP
#define FILTERINGGENERAL_HPP

#include "imu.hpp"

// -1 = FAILED
// 0 = SUCCESS
// 1 = Old Data
struct FilteringError_t {
  int errorCode;
};

class FilteringGeneral {
public:
  static IMU *imuObj;
  static FilteringError_t Init();
  static IMUData_t GetRawIMU();
};


#endif
