#include "airspeed.hpp"

namespace Airspeed
{

MS4525DO::MS4525DO(I2C_HandleTypeDef* hi2c): i2c(hi2c)
{
}

bool MS4525DO::init()
{
    update_data();
    return airspeedData.status == STATUS_FAIL;
}

void MS4525DO::get_data(airspeed_data_t* airspeedDataOut)
{
    airspeedDataOut = &airspeedData;
    update_data();
}

void MS4525DO::update_data(void)
{
    read_data();
    update_pressure();
    update_temp();
    update_airspeed();
}

void MS4525DO::update_pressure(void)
{
    float pressure = pressureReading - 0.1 * MAX_COUNT_PRESSURE;
    pressure *= (MAX_PRESSURE - MIN_PRESSURE)/(0.8 * MAX_COUNT_PRESSURE);
    pressure += MIN_PRESSURE;

    airspeedData.pressure = pressure;
} 

void MS4525DO::update_temp(void)
{
    float slope = (MAX_TEMP - MIN_TEMP)/MAX_COUNT_TEMP;
    
    airspeedData.temperature = slope * tempratureReading + MIN_TEMP;
}

void MS4525DO::update_airspeed(void)
{
    // Ptotal = Pstatic + Pdynamic
    // Measuring Pdynamic
    // Pdynamic = 1/2 * rho * velocity^2
    // velocity = sqrt( 2/rho * Pdynamic )
    // PSI = 6894.76 Pa
    // Pa = kg / (ms^2)

    float pressure = PSI_TO_PA(airspeedData.pressure);
    float airspeed = sqrt( 2/RHO * pressure );
}

void MS4525DO::read_data(void)
{
    HAL_StatusTypeDef ret = HAL_I2C_Master_Receive(
        i2c, 
        (MS4525DO_I2C_ADDR << 1 | 0x01),
        rxBuffer,
        4,
        50
    );

    if (ret != HAL_OK)
    {
        airspeedData.status = STATUS_FAIL;
        return;
    }

    // Parsing status bits from recieve buffer
    uint8_t statusMask = 0x3FU; // 0b00111111
    
    airspeedData.status = (sensor_status_t) ((rxBuffer[0] & ~statusMask) >> 6);
    
    pressureReading = uint16_t((rxBuffer[0] & statusMask) << 8 | rxBuffer[1]);

    uint8_t tempMask = 0xE0U; // 0b11100000
    tempratureReading = uint16_t(rxBuffer[2] << 8 | rxBuffer[3] & tempMask);
}


}