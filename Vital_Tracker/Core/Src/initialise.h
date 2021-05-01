/*
 * initialise.h
 * Description: Contains the header files and method declaration required for initialise.c
 *
 *
 *      Author: sharath and Chaithra
 */

#ifndef SRC_INITIALISE_H_
#define SRC_INITIALISE_H_

#include "main.h"


/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
RTC_HandleTypeDef hrtc;
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;


/* Private define -----------------------------------------------------------*/
#define TEMP_SENSOR_PORT GPIOA
#define TEMP_SENSOR_PIN GPIO_PIN_4

#define LED_PORT GPIOB
#define LED_PIN GPIO_PIN_2

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_RTC_Init(void);
void MX_SPI2_Init(void);
void MX_I2C1_Init(void);
void MX_SPI1_Init(void);

#endif /* SRC_INITIALISE_H_ */
