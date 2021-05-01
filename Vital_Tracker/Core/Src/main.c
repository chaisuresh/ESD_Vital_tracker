
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */


/*
 * The main function contains all the initialisation calls and calls to different peripherals
 * The main function also has NRF begin and configurations required for radio communication
 *
 */
 
 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include "MY_NRF24.h"
#include "nRF24L01.h"
#include "heart_oxy_module.h"
#include "temperature.h"
#include "initialise.h"


/* Private user code ---------------------------------------------------------*/
uint64_t TxpipeAddrs = 0x68;


/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) 
{
	

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	
	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_RTC_Init();
	MX_SPI2_Init();
	MX_I2C1_Init();
	MX_SPI1_Init();
	
	//Begin the NRF radio communication
	NRF24_begin(GPIOC, GPIOB, GPIO_PIN_12, GPIO_PIN_6, hspi2);

	//**** TRANSMIT - ACK ****//
	NRF24_stopListening(); //set this NRF module as transmitter
	NRF24_openWritingPipe(TxpipeAddrs); //we write to address 0x68 which the receiver is listening to
	NRF24_setAutoAck(true); //we set auto acknowledge
	NRF24_setChannel(76); //same channel set at receiver
	NRF24_setPayloadSize(32); //We set the number of bytes per transmission
	NRF24_enableAckPayload(); // we set auto acknowledge for payload

	// Starting up the heart-oxy module
	Heart_Power_On();

	// Initializing commands for the heart-oxy module
	Heart_Init();


	/* Infinite loop */
	
	while (1)
	{	
		
		callibrate_temperature_value();	//read and send temperature sensor
		callibrate_heart_oxy_values(); //read and send heart and oxy sensor
			
		//Toggle LED at 1sec
		HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
		HAL_Delay(1000);
	}
	
}



/****************************** END OF FILE ******************************************/
