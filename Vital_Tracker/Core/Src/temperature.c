/*
 * temperature.c
 * Description: This file contains the functions for reading the temperature
 * We refered the datasheet of the temperature sensor- ADT7310TRZ for the commands
 * 
 * Author: sharath & chaithra
 */
 

 #include "temperature.h"
#include "initialise.h"
 #include "MY_NRF24.h"  //Radio header to send temperature data over radio
 
 char myTxData[32];
 /* 
  * A number of buffers are initialised to store the respective commands to the sensor 
  * Commands are refered from the user guide below
  * https://www.analog.com/media/en/technical-documentation/data-sheets/ADT7310.pdf
  * 
  */
 uint8_t tem_buf[2] = { 0x54, 0x50 };
 uint8_t tem_buf2[2] = { 0xFF, 0xFF };
 uint8_t tem_buf1[2] = { 0x00, 0x00 };
 
 


/**
 * Sequence of commands that read the temperature from
 * the temperature sensor
 *
 * @params void
 *
 * @return void
 */
void read_temperature()
 {
	 // Pull the enable pin low for temperature  sensor
	HAL_GPIO_WritePin(TEMP_SENSOR_PORT, TEMP_SENSOR_PIN, RESET);
	 
	 //Flush the buffer of temperature sensor by sending FF command
	HAL_SPI_Transmit(&hspi1, tem_buf2, 2, 50);
	 
	// command to set in continuous read mode
	HAL_SPI_Transmit(&hspi1, &tem_buf[0], 1, 50);

	HAL_Delay(20);

	// Read the temperature buffer
	HAL_SPI_Receive(&hspi1, tem_buf1, 2, 50);

	HAL_Delay(20);

	// Stop the continuous read mode
	HAL_SPI_Transmit(&hspi1, &tem_buf[1], 1, 50);

	HAL_Delay(20);
}


/**
 * Calculates the temp sensor and sends it over the radio
 *
 * @params void
 *
 * @return void
 */
void callibrate_temperature_value(void)
 {

	float temp=0;
	 
	 //start reading temperature
	 read_temperature();


	
	// the first byte is the decimal value of temperature
	// the second byte is the 0.0625 sensitive floating number of the measured temperature
	//second byte is masked for the last 5 bits as we are interested in the 13-bit temperature output
	temp = (tem_buf1[0]) + ((tem_buf1[1] && 0x01F) * 0.0625);
	
	
		// Radio module Transmission, 
		//sends measured temperature over the radio module
		sprintf(myTxData, "temp= %f", temp);

		if (NRF24_write(myTxData, sizeof(myTxData))) 
		{
			NRF24_read(AckPayload, 3);
			char myDataack[80];
			sprintf(myDataack, "AckPayload:  %s \r\n", AckPayload);
		}
}
