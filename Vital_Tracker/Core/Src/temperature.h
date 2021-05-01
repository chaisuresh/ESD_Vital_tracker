/*
 * temperature.h
 * This file contains the header file for functions in tempeature sensor
 * 
 * Author: sharath & chaithra
 */

#ifndef SRC_TEMPERATURE_H_
#define SRC_TEMPERATURE_H_



/**
 * Sequence of commands that read the temperature from
 * the temperature sensor
 *
 * @params void
 *
 * @return void
 */
void read_temperature();

/**
 * Calculates the temp sensor and sends it over the radio
 *
 * @params void
 *
 * @return void
 */
void callibrate_temperature_value(void);

#endif /* SRC_TEMPERATURE_H_ */
