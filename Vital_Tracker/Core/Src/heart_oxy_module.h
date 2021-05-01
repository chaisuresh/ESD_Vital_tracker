/*
 * heart_oxy_module.h
 * Description: This file contains the header files for all the heart and oxy meter related methods
 * 
 *
 * Author: sharath & chaithra
 */

#ifndef SRC_HEART_OXY_MODULE_H_
#define SRC_HEART_OXY_MODULE_H_


#define Heart_Addr ((0x55)<<1) //slave address for heart and oxy sensor

/**
 * @brief Sequence of commands that start up the heart-oxy module
 *
 * @param void
 *
 * @return void
 */
void Heart_Power_On();

/**
 * @brief Sequence of commands that initialize the heart-oxy module
 * This method includes commands that start the WHRM algorithm,
 * Set the device mode, read number of samples, etc.
 *
 * @param void
 *
 * @return void
 */
void Heart_Init();

/**
 * @brief Sequence of commands that perform a read operation
 * from the module
 *
 * @param void
 *
 * @return void
 */
void Heart_Read_Samples();


/**
 * @brief calculates the readings from the received data from the sensor 
 * and send heart & oxygen saturation level over the radio
 *
 * @param void
 *
 * @return void
 */
void callibrate_heart_oxy_values(void);

#endif /* SRC_HEART_OXY_MODULE_H_ */
