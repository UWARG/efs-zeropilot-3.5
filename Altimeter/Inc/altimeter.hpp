#ifndef INC_ALTIMETER_HPP_
#define INC_ALTIMETER_HPP_

#include "stm32l5xx_hal.h"
#include "cmsis_os.h"
#include <cstdint>
#include <cmath>


#define RESET 0x1E

/*PROM Addresses */
#define PROM_READ_ADDRESS_0 0xA0
#define PROM_READ_ADDRESS_1 0xA2
#define PROM_READ_ADDRESS_2 0xA4
#define PROM_READ_ADDRESS_3 0xA6
#define PROM_READ_ADDRESS_4 0xA8
#define PROM_READ_ADDRESS_5 0xAA
#define PROM_READ_ADDRESS_6 0xAC
#define PROM_READ_ADDRESS_7 0xAE


/*digital pressure/temperature commands at different sampling rates*/
#define CONVERT_D1_OSR_256 0x40
#define CONVERT_D1_OSR_512 0x42
#define CONVERT_D1_OSR_1024 0x44
#define CONVERT_D1_OSR_2048 0x46
#define CONVERT_D1_OSR_4096 0x48
#define CONVERT_D2_OSR_256 0x50
#define CONVERT_D2_OSR_512 0x52
#define CONVERT_D2_OSR_1024 0x54
#define CONVERT_D2_OSR_2048 0x56
#define CONVERT_D2_OSR_4096 0x58

#define ADC_READ 0x0

#define TIMEOUT 1 << 31

typedef struct cal_coeffs{
	uint16_t sens;
	uint16_t off;
	uint16_t tcs;
	uint16_t tco;
	uint16_t t_ref;
	uint16_t temp_sens;
}cal_coeffs;



/* To get temperature/pressure/altitude values, call (step 1) AltDevice, then (step 2) AltInit once.
 * Then before getting (step 4) pressure/temp/altitude, (step 3) calculateTempPres should be called beforehand.
 *  */

class AltDevice{
public:
	/**
	* Constructor for the AltDevice class
	 *
	* @param ps_port -> protocol select port
	* @param ps_pin -> protocol select pin
	 *
	* @return none
	 */
	AltDevice(GPIO_TypeDef *ps_port, uint16_t ps_pin);

	/**
	* This function sends the reset command, populates the cal_coeffs
	* struct members with the calibration coefficients in the sensor PROM,
	* and gets the current elevation above sea level.
	 *
	* @param ps_port -> protocol select port
	* @param ps_pin -> protocol select pin
	* @param spi_handle -> hspix handle
	 *
	* @return none
	 */
	void altInit(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_port, uint16_t cs_pin);

	/**
	* Calculates datasheet variable values needed for temperature and pressure,
	* and calculates temperature and pressure. It
	 *
	* @param ps_port -> protocol select port
	* @param ps_pin -> protocol select pin
	* @param spi_handle -> hspix handle
	 *
	* @return none
	 */
	void calculateTempPres(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_port, uint16_t cs_pin);


	/**
	* @return the current pressure value
	 */
	float getPressure();

	/**
	* @return the current temperature value
	 */
	float getTemperature();

	/**
	* @return the current altitude above sea level.
	 */
	float getAltAboveSeaLvl();

	/**
	* @return the current altitude above zero point.
	 */
	float getAltitude();


private:
	cal_coeffs coeffs_;
	float base_elev_;
	float height_;
	float temp_;
	float pres_;

	/**
	* Sends the reset command to the barometer.
	 *
	* @param ps_port -> protocol select port
	* @param ps_pin -> protocol select pin
	* @param spi_handle -> hspix handle
	 *
	* @return none
	 */
	void reset(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_port, uint16_t cs_pin);

	/**
	* Populates the struct with calibration coefficients.
	 *
	* @param ps_port -> protocol select port
	* @param ps_pin -> protocol select pin
	* @param spi_handle -> hspix handle
	 *
	* @return none
	 */
	void getConvCoeffs(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_port, uint16_t cs_pin);

	/**
	* Reads user-specified calibration coefficient from the PROM
	 *
	* @param ps_port -> protocol select port
	* @param ps_pin -> protocol select pin
	* @param spi_handle -> hspix handle
	 *
	* @return calibration data
	 */
	uint16_t promRead(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_port, uint16_t cs_pin, uint8_t address);

	/**
	* Reads uncompenssated temperature/pressure value from altimeter.
	 *
	* @param ps_port -> protocol select port
	* @param ps_pin -> protocol select pin
	* @param spi_handle -> hspix handle
	 *
	* @return uncompensated pressure/temperature value
	 */
	uint32_t uncompensatedPressureTemperature(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *cs_port, uint16_t cs_pin, uint8_t conversion_command);
};


#endif /* INC_ALTIMETER_HPP_ */
