#include "FilteringGeneral.hpp"
#include "imu.hpp"
#include <string.h>

namespace FilteringGeneral { 

IMU* FilteringGeneral::imuObj = nullptr;

FilteringError_t FilteringGeneral::Init() {
  FilteringError_t ret;
  ret.errorCode = 0;
  
  imuObj = &BMX160::getInstance();
  
  return ret;
}

IMUData_t FilteringGeneral::GetRawIMU() {
  IMUData_t imuData;
  imuObj->GetResult(imuData);
  
  IMUData_t imuOutput;

  std:memcpy(&imuOutput, &imuData, sizeof(IMUData_t));

  return imuOutput;
}

}
