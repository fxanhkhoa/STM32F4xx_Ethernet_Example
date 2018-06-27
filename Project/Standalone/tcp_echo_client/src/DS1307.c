#include "DS1307.h"

/* private variables */
I2C_InitTypeDef I2C_InitStructure;
DATE_TYPE saveTime;

void DS1307Init() 
{
	I2CInit();
	//I2C_ITConfig(I2C2, I2C_IT_BUF | I2C_IT_EVT, ENABLE);
}

void I2CInit() 
{

	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);

	/* Enable UART clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_I2C3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_I2C3);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = DS1307_I2C_MASTER_ADDRESS;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

	/* Enable I2C */
	I2C_Init(I2C3, &I2C_InitStructure);
	I2C_Cmd(I2C3, ENABLE);
}

DATE_TYPE DS1307ReadTime()
{
	uint8_t index;
	uint8_t values[8] = { 0 };
	DATE_TYPE saveTime;
	
	I2C_AcknowledgeConfig(DS1307_I2C, ENABLE);

	// Start setting register
	I2C_GenerateSTART(DS1307_I2C, ENABLE);
	while (!I2C_CheckEvent(DS1307_I2C, I2C_EVENT_MASTER_MODE_SELECT))
		;

	I2C_Send7bitAddress(DS1307_I2C, DS1307_I2C_SLAVE_ADDRESS, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(DS1307_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(DS1307_I2C, 0x00);
	while (!I2C_CheckEvent(DS1307_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTART(DS1307_I2C, ENABLE);
	while (!I2C_CheckEvent(DS1307_I2C, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(DS1307_I2C, DS1307_I2C_SLAVE_ADDRESS, I2C_Direction_Receiver);
	while (!I2C_CheckEvent(DS1307_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	for (index = 0; index < 8; index++) {
		while (!I2C_CheckEvent(DS1307_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
			;
		values[index] = I2C_ReceiveData(DS1307_I2C);
		if (index == 6) {
			I2C_NACKPositionConfig(DS1307_I2C, I2C_NACKPosition_Current);
			I2C_AcknowledgeConfig(DS1307_I2C, DISABLE);
		}
	}
	
	I2C_GenerateSTOP(DS1307_I2C, ENABLE);
	while (I2C_GetFlagStatus(DS1307_I2C, I2C_FLAG_STOPF));
	
	saveTime.sec1 = (values[0] & 0xF0) >> 4;
	saveTime.sec0 = values[0] & 0x0F;
	saveTime.min1 = (values[1] & 0xF0) >> 4;
	saveTime.min0 = values[1] & 0x0F;
	saveTime.hour1 = (values[2] & 0x30) >> 4;
	saveTime.hour0 = values[2] & 0x0F;
	saveTime.dayOfWeek = values[3] & 0x07;
	saveTime.day1 = (values[4] & 0x30) >> 4;
	saveTime.day0 = values[4] & 0x0F;
	saveTime.month1 = (values[5] & 0x10) >> 4;
	saveTime.month0 = values[5] & 0x0F;
	saveTime.year1 = (values[6] & 0xF0) >> 4;
	saveTime.year0 = values[6] & 0x0F;
	
	return saveTime;
}

void DS1307SetTime()
{
	uint8_t index;
	uint8_t values[8] = { 0x00, 0x52, 0x01, 0x04, 0x28, 0x06, 0x18, 0x10 };

	I2C_AcknowledgeConfig(DS1307_I2C, ENABLE);

	// Start setting register
	I2C_GenerateSTART(DS1307_I2C, ENABLE);
	while (!I2C_CheckEvent(DS1307_I2C, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(DS1307_I2C, DS1307_I2C_SLAVE_ADDRESS, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(DS1307_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(DS1307_I2C, 0x00);
	while (!I2C_CheckEvent(DS1307_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	for (index = 0; index < 8; index++) {
		I2C_SendData(DS1307_I2C, values[index]);
		while (!I2C_CheckEvent(DS1307_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}

	I2C_NACKPositionConfig(DS1307_I2C, I2C_NACKPosition_Current);
	I2C_AcknowledgeConfig(DS1307_I2C, DISABLE);

	I2C_GenerateSTOP(DS1307_I2C, ENABLE);

	while (I2C_GetFlagStatus(DS1307_I2C, I2C_FLAG_STOPF));
}

char* DS1307GetTimeString() 
{
	saveTime = DS1307ReadTime();
	char *time = (char *) calloc(20, sizeof(char));
	time[0] = (char)saveTime.hour1 + 0x30;
	time[1] = (char)saveTime.hour0 + 0x30;
	time[2] = (char)0x3A;
	time[3] = (char)saveTime.min1 + 0x30;
	time[4] = (char)saveTime.min0 + 0x30;
	time[5] = (char)0x3A;
	time[6] = (char)saveTime.sec1 + 0x30;
	time[7] = (char)saveTime.sec0 + 0x30;
	time[8] = (char)0x3A;
	time[9] = (char)saveTime.dayOfWeek + 0x30;
	time[10] = (char)0x3A;
	time[11] = (char)saveTime.day1 + 0x30;
	time[12] = (char)saveTime.day0 + 0x30;
	time[13] = (char)0x3A;
	time[14] = (char)saveTime.month1 + 0x30;
	time[15] = (char)saveTime.month0 + 0x30;
	time[16] = (char)0x3A;
	time[17] = (char)saveTime.year1 + 0x30;
	time[18] = (char)saveTime.year0 + 0x30;
	
	return time;
}
