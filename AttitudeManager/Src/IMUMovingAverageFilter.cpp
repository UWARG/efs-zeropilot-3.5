#include "IMUMovingAverageFilter.hpp"
#include "imu.hpp"
#include "MovingAverage.hpp"

IMUMovingAverageFilter::IMUMovingAverageFilter() {
  imuObj = &BMX160::getInstance();
  magxVals = Window<float>();
  magyVals = Window<float>();
  magzVals = Window<float>();
  accxVals = Window<float>();
  accyVals = Window<float>();
  acczVals = Window<float>();
  gyrxVals = Window<float>();
  gyryVals = Window<float>();
  gyrzVals = Window<float>();
}

MovingAverageError_t IMUMovingAverageFilter::Update() {
  IMUData_t currentIMUData;
  imuObj->GetResult(currentIMUData);

  if (!currentIMUData.isDataNew) {
    // We could log here, ideally this case shouldn't occur and we should slow 
    // down the refresh rate if we encounter this often.
    return OLD_VALUES;
  }

  magxVals.update(currentIMUData.magx);
  magyVals.update(currentIMUData.magy);
  magzVals.update(currentIMUData.magz);
  accxVals.update(currentIMUData.accx);
  accyVals.update(currentIMUData.accy);
  acczVals.update(currentIMUData.accz);
  gyrxVals.update(currentIMUData.gyrx);
  gyryVals.update(currentIMUData.gyry);
  gyrzVals.update(currentIMUData.gyrz);
  
  return SUCCESS;
}

// Populates FilteredIMUData.
MovingAverageError_t IMUMovingAverageFilter::GetAverage(uint8_t windowSize, IMUData_t* FilteredIMUData) {

  int checksum;

  checksum += magxVals.getAverage(windowSize, &FilteredIMUData->magx);
  checksum += magyVals.getAverage(windowSize, &FilteredIMUData->magy);
  checksum += magzVals.getAverage(windowSize, &FilteredIMUData->magz);
  checksum += accyVals.getAverage(windowSize, &FilteredIMUData->accx);
  checksum += acczVals.getAverage(windowSize, &FilteredIMUData->accy);
  checksum += accxVals.getAverage(windowSize, &FilteredIMUData->accz);
  checksum += gyrxVals.getAverage(windowSize, &FilteredIMUData->gyrx);
  checksum += gyryVals.getAverage(windowSize, &FilteredIMUData->gyry);
  checksum += gyrzVals.getAverage(windowSize, &FilteredIMUData->gyrz);

  return checksum == 0 ? FAILURE : SUCCESS;
}
