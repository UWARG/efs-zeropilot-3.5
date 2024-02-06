#ifndef AIRSPEED_HPP
#define AIRSPEED_HPP

#include "i2c.h"
#include <cstdint>

#define MS4525DO_I2C_ADDR1 0x28
#define MS4525DO_I2C_ADDR2 0x36
#define MS4525DO_I2C_ADDR3 0x46

#define MAX_TEMP 150
#define MIN_TEMP -50

#define MAX_PRESSURE 150
#define MIN_PRESSURE -150

namespace Airspeed
{

typedef enum {
  STATUS_GOOD = 0x00U,
  STATUS_RESERVED = 0x01U,
  STATUS_STALE = 0x02U,
  STATUS_FAIL = 0x03U
} sensor_status_t;

struct airspeed_data_t{
  float airspeed {0.0f}; // m/s
  float pressure {0.0f}; // psi
  float temperature {0.0f}; // degrees C

  sensor_status_t status; // status of sensor
};

class MS4525DO
{
public:
  MS4525DO(I2C_HandleTypeDef* hi2c);
  
  bool init(void);
  void get_data(airspeed_data_t* airspeedData);  
  void update_data(void);
  
private:
  airspeed_data_t airspeedData;

  I2C_HandleTypeDef* i2c;
  
  uint8_t rxBuffer[4];
  int16_t pressureReading;
  int16_t tempReading;
 
  void update_pressure(void);
  void update_temp(void);
  
  void update_airspeed(void);
  
  void read_data(void);
};

}

#endif /* AIRSPEED_HPP */
