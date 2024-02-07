#ifndef AIRSPEED_HPP
#define AIRSPEED_HPP

#include "i2c.h"
#include "math.h"
#include <cstdint>

#define MS4525DO_I2C_ADDR 0x28

#define MAX_TEMP 150 // Deg C
#define MIN_TEMP -50 // Deg C

#define MAX_COUNT_TEMP 0x07FF

// 001 model, so range is 1 PSI
#define PRESSURE_RANGE 1 // PSI

// Differential pressure type model
#define MAX_PRESSURE PRESSURE_RANGE
#define MIN_PRESSURE -PRESSURE_RANGE

#define MAX_COUNT_PRESSURE 0x3FFF

#define RHO 1.225 // kg / m^3
#define PSI_PA_CONSTANT 6894.747

#define PSI_TO_PA(pressure) (pressure * PSI_PA_CONSTANT)


namespace Airspeed
{

typedef enum {
  STATUS_GOOD = 0x00U,
  STATUS_RESERVED = 0x01U,
  STATUS_STALE = 0x02U,
  STATUS_FAIL = 0x03U
} sensor_status_t;

struct airspeed_data_t{
  float airspeed {0.0f}; // m / s
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
  uint16_t pressureReading;
  uint16_t tempratureReading;
 
  void update_pressure(void);
  void update_temp(void);
  
  void update_airspeed(void);
  
  void read_data(void);
};

}

#endif /* AIRSPEED_HPP */
