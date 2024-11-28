/*
 * ms5611_constants.h
 *
 * The following are constants used for calculations
 * involved in returning the pressure (mbar), temperature (celsius)
 * and altitude (meters).
 *
 * Error handling: The public functions returning some measurement
 * are based on a hierarchy of functions. If the microcontroller
 * fails to communicate with the sensor at any point of that function
 * hierarchy, the functions returning these measurements will return
 * FLT_MAX. Otherwise, they will return the desired value.
 *
 */

#ifndef INC_MS5611_CONSTANTS_H_
#define INC_MS5611_CONSTANTS_H_

	constexpr float TWO_POW_7 = 128.0f;
	constexpr float TWO_POW_8 = 256.0f;
	constexpr float TWO_POW_15 = 32768.0f;
	constexpr float TWO_POW_16 = 65536.0f;
	constexpr float TWO_POW_21 = 2097152.0f;
	constexpr float TWO_POW_23 = 8388608.0f;
	constexpr float TWO_POW_31 = 2147483648.0f;

	//These constants are for the barometric formula
	//https://en.wikipedia.org/wiki/Barometric_formula
	constexpr float REFERENCE_TEMP = 288.15f; //Ref temperature in Kelvins.
	constexpr float TEMP_LAPSE_RATE = 0.0065f;
	constexpr float EXP_GMRL = 5.2558;

	constexpr float REFERENCE_PRESSURE = 101325.0f; /* in Pa */




#endif /* INC_MS5611_CONSTANTS_H_ */
