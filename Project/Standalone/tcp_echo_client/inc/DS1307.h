#ifndef _DS1307_H_
#define _DS1307_H_

#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"
#include <stdlib.h>

/* Define */
#define DS1307_I2C_SLAVE_ADDRESS    0xD0
#define DS1307_I2C_MASTER_ADDRESS   0x00
#define DS1307_I2C						I2C3
#define DS_1307_SCL		GPIO_Pin_6
#define DS_1307_SDA		GPIO_Pin_7
#define DS_1307_RCC		RCC_APB1Periph_I2C3
#define DS_1307_PORT_RCC  RCC_AHB1Periph_GPIOA
#define DS_1307_PORT  GPIOB

typedef struct 
{
	uint8_t sec1;
	uint8_t sec0;
	uint8_t min1;
	uint8_t min0;
	uint8_t hour1;
	uint8_t hour0;
	uint8_t day1;
	uint8_t day0;
	uint8_t month1;
	uint8_t month0;
	uint8_t year1;
	uint8_t year0;
	uint8_t dayOfWeek;
}DATE_TYPE;

void DS1307Init();
void I2CInit();
DATE_TYPE DS1307ReadTime();
void DS1307SetTime();
char* DS1307GetTimeString();

#endif
