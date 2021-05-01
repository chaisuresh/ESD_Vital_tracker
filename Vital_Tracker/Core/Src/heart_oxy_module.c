/*
 * heart_oxy_module.c
 * Description: This file contains turning the sensor,
 * Initialising the heart and oxy sensor and reading samples from the sensor
 * Reference: MAX32664 user guide had a lot of information for initialising the sensor and a table of commands and response to do these commands
 *  We sent the same command to the slave ( address- 0x55) and read the samples
 * 
 *
 * Author: sharath & chaithra
 */

#include "heart_oxy_module.h" 
#include "initialise.h"

#include "MY_NRF24.h" //Radio header to send sensor data over radio

char myTxData1[32];
 
 //variables for heart, oxy
 uint16_t heart = 0, oxy = 0;
 
 /* 
  * A number of buffers are initialised to store the respective commands to the sensor 
  * Commands are refered from the user guide below
  * https://pdfserv.maximintegrated.com/en/an/ug6806.pdf
  */
 
 uint8_t version_cmd[3] = { 0xFF, 0x03 }; //version
 uint8_t exit_bootloader_cmd[3] = { 0x01, 0x00, 0x00 }; //exit bootloader
 uint8_t device_mode_cmd[2] = { 0x02, 0x00 }; //device mode
 uint8_t set_data_mode_cmd[3] = { 0x10, 0x00, 0x03 }; // set data mode
 uint8_t sample_read_cmd[2] = { 0x12, 0x00 }; //nu of samples
 uint8_t enable_sensor_cmd[3] = { 0x44, 0x03, 0x01 }; //enable sensor
 uint8_t whrm_enable[3] = { 0x52, 0x02, 0x01 }; // algorithm- wrist enable
 uint8_t read_status[2] = { 0x00, 0x00 }; //read sensor hub status
 uint8_t read_data[2] = { 0x12, 0x01 }; //read data
 uint8_t set_output_mode[3] = { 0x10, 0x01, 0x01 }; //size limit


 /*
  * A number of buffers are initialised to receive the responses from the sensor for the command sent
  */
 uint8_t rcv = 0x01;
 uint8_t rcv1[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
 uint8_t rcv2[3] = { 0x00, 0x00, 0x00 };
 uint8_t buf1[3] = { 0x02, 0x00, 0x00 }; 
 uint8_t rcv3[18] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };



/*
 * @brief Sequence of commands that strat up the heart-oxy module
 *
 * @param void
 *
 * @return void
 */
void Heart_Power_On() 
{
	
	/* We have a routine to follow to turn the Heart sensor Power ON 
	*  we pull the RESET low for 10ms, and for a minimum of 1ms, we pull MFIO high
	*  (MFIO- Multifunction I/O )
	*  We pull the RESET high after this
	*  This will move the sensor to application mode ( from bootloader mode)
	*  The sensor should remain like this for a minimum of 500ms after this, to ensure application mode
	*
	*	 GPIO_PIN_7 is RESET pin
	* 	 GPIO_PIN_6 is MFIO pin	
	*/

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, RESET); //reset pin
	HAL_Delay(5);
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, SET); //MFIO pin
	HAL_Delay(5);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, SET); //reset pin
	HAL_Delay(1000);

}




/*
 * @brief Sequence of commands that initialize the heart-oxy module
 * This method includes commands that start the WHRM algorithm,
 * Set the device mode, read number of samples, etc.
 *
 * @param void
 *
 * @return void
 */
void Heart_Init() 
{

	// Command to get version number
	HAL_I2C_Master_Transmit(&hi2c1, Heart_Addr, version_cmd, 2, 50);
	HAL_Delay(1);
	// Receiving the version number
	HAL_I2C_Master_Receive(&hi2c1, Heart_Addr | 0x01, rcv1, 5, 50);
	HAL_Delay(1);

	// Command to for the module to exit the bootloader mode
	HAL_I2C_Master_Transmit(&hi2c1, Heart_Addr, exit_bootloader_cmd, 3, 50);
	HAL_Delay(1);
	// Receiving the success response
	HAL_I2C_Master_Receive(&hi2c1, Heart_Addr | 0x01, &rcv, 1, 50);
	HAL_Delay(1);

	// Command to check the device mode
	HAL_I2C_Master_Transmit(&hi2c1, Heart_Addr, device_mode_cmd, 2, 50); // read device mode
	HAL_Delay(1);
	/* Response that shows device mode;
	 * 0x00: application mode
	 * 0x02: reset
	 * 0x08 Bootloader mode
	 */
	HAL_I2C_Master_Receive(&hi2c1, Heart_Addr | 0x01, &buf1[0], 1, 50);
	HAL_Delay(1);


	//Re-using the same variable and initialising it to some random value, So we can check for a response of 0
	rcv = 0x02;

	// Set the data mode; Set the threshold for the FIFO interrupt bit/pin
	HAL_I2C_Master_Transmit(&hi2c1, Heart_Addr, set_output_mode, 3, 50);
	HAL_Delay(1);
	// Success response
	HAL_I2C_Master_Receive(&hi2c1, Heart_Addr | 0x01, &rcv, 1, 50);
	HAL_Delay(1);

	//Re-using the same variable and initialising it to some random value, So we can check for a response of 0
	rcv = 0x02;

	// Set the data mode; Set the threshold for the FIFO interrupt bit/pin
	HAL_I2C_Master_Transmit(&hi2c1, Heart_Addr, set_data_mode_cmd, 3, 50);
	HAL_Delay(1);
	// Success response
	HAL_I2C_Master_Receive(&hi2c1, Heart_Addr | 0x01, &rcv, 1, 50);
	HAL_Delay(1);
	

	//Re-using the same variable and initialising it to some random value, So we can check for a response of 0
	rcv = 0x03;
	
	// Command to enable the sensor module
	HAL_I2C_Master_Transmit(&hi2c1, Heart_Addr, enable_sensor_cmd, 3, 50); //enable sensor
	HAL_Delay(1);
	/// Success response
	HAL_I2C_Master_Receive(&hi2c1, Heart_Addr | 0x01, &rcv, 1, 50);
	HAL_Delay(1);

	//Re-using the same variable and initialising it to some random value, So we can check for a response of 0
	rcv = 0x04;

	// Enable the WHRM, MaximFast algorithm
	HAL_I2C_Master_Transmit(&hi2c1, Heart_Addr, whrm_enable, 3, 50); //wrist enable
	HAL_Delay(1);
	// Sucess response
	HAL_I2C_Master_Receive(&hi2c1, Heart_Addr | 0x01, &rcv, 1, 50);
	HAL_Delay(1);

	// command to read the sensor hub status
	HAL_I2C_Master_Transmit(&hi2c1, Heart_Addr, read_status, 2, 50);
	HAL_Delay(1);
	// 1 byte response
	HAL_I2C_Master_Receive(&hi2c1, Heart_Addr | 0x01, rcv2, 3, 50);
	HAL_Delay(1);
}


/*
 * @brief Sequence of commands that perform a read operation
 * from the module
 *
 * @param void
 *
 * @return void
 */
void Heart_Read_Samples() 
{
	
	// First we read the number of samples that the sensor collected to give the heart and oxy readings
	HAL_I2C_Master_Transmit(&hi2c1, Heart_Addr, sample_read_cmd, 2, 50); //nu of samples
	HAL_Delay(1);
	HAL_I2C_Master_Receive(&hi2c1, Heart_Addr | 0x01, buf1, 3, 50);
	HAL_Delay(1);

	//Then we read the heart rate sensor, oxymeter readings in a 16byte array
	// This 16 byte also gives absorption rate of IR LED and RED LED , But we neglect this reading
	HAL_I2C_Master_Transmit(&hi2c1, Heart_Addr, read_data, 2, 50); //read data
	HAL_Delay(1);
	HAL_I2C_Master_Receive(&hi2c1, Heart_Addr | 0x01, rcv3, 18, 50);
	HAL_Delay(1);
}


/*
 * @brief calculates the readings from the received data from the sensor 
 * and send heart & oxygen saturation level over the radio
 *
 * @param void
 *
 * @return void
 */
void callibrate_heart_oxy_values(void)
{
	//read the data from sensor
	Heart_Read_Samples();
	
	//the 12th and 13th byte of the received data makes up for the measured heart rate 
	heart = ((rcv3[12] << 8) + rcv3[13]) / 100;
	//the 15th and 16th byte of the received data makes up for the measured oxygen rate 
	oxy = ((rcv3[15] << 8) + rcv3[16]) / 100;
	
	//sends the heart rate and oxy sensor data over the radio using NRF write
	sprintf(myTxData1, " heart=%d  oxy = %d ", heart, oxy);
		if (NRF24_write(myTxData1, sizeof(myTxData1))) {
			NRF24_read(AckPayload, 3);
			char myDataack[80];
			sprintf(myDataack, "AckPayload:  %s \r\n", AckPayload);
		}
		
		
}
