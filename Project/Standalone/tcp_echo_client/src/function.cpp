#include "function.h"

//private variables
unsigned char outPutText[30];
DATE_TYPE timeNow;

/*
Function Name: OpenDoor(number)

Description: Open door from 0 to 3

Argument: number of door

Return: no output

Note: none
*/
void OpenDoor(int number)
{
	if (number == 0)
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_7);
	}
	else if (number == 1)
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_8);
	}
	else if (number == 2)
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_9);
	}
	else if (number == 3)
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_10);
	}
}

/*
Function Name: CloseDoor(number)

Description: Close door from 0 to 3

Argument: number of door

Return: no output

Note: none
*/
void CloseDoor(int number)
{
	if (number == 0)
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_7);
	}
	else if (number == 1)
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_8);
	}
	else if (number == 2)
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_9);
	}
	else if (number == 3)
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_10);
	}
}

/*
Function Name: led_toggle

Description: To toggle LED in Pin 12 (LED GREEN)

Argument: no input

Return: no output

Note: none
*/
void led_toggle(void)
{
	/* Read LED output (GPIOA PIN8) status */
	uint8_t led_bit = GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_12);
			 
	/* If LED output set, clear it */
	if(led_bit == (uint8_t)Bit_SET)
	{
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
	}
	/* If LED output clear, set it */
	else
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
	}
}

/*
*	Input char * : string need to split
*	Output char * : string of RFID
*/

char * IDADD_READ_RFID(char *s)
{
	char *temp = new char[5];
	memcpy(temp, &s[9], 4);
	temp[4] = '\0';
	return temp;
}

char * IDADD_READ_TIME(char *s)
{
	char *temp = new char[12];
	memcpy(temp, &s[17], 12);
	temp[11] = '\0';
	return temp;
}

char * IDADD_READ_DOOR(char *s)
{
	char *temp = new char[1];
	temp[0] = s[28];
	return temp;
}

/*
Function Name: CheckOpenDoor

Description: Compare ID, Time, Door return mode

Argument: char *s

Return: char

Note: none
*/
uint8_t CheckOpenDoor(char *s)
{
	//U_Print_Char(USART1,s);
	unsigned char mode_temp = NONE;
	int j = 0,flag = 0;
	char RFID[6];
	uint8_t hourFrom, hourTo;
	uint8_t minuteFrom, minuteTo;
	uint16_t quantity = GetQuantity();
	outPutText[0] = '\0';
	for (j = 0; j < quantity; j++)
	{
		mode_temp = 1; // true state
		timeNow = DS1307ReadTime(); // Get current time
		//ID[4] = '\0';
		//U_Print_Char(USART1, ID);
		//U_Print_Char(USART1, "\n");
		
		//Read RFID
		RFID[0] = EEPROM_readByte(START_OF_RFID_USER + j*NUMBER_OF_BLOCK + 0);
		RFID[1] = EEPROM_readByte(START_OF_RFID_USER + j*NUMBER_OF_BLOCK + 1);
		RFID[2] = EEPROM_readByte(START_OF_RFID_USER + j*NUMBER_OF_BLOCK + 2);
		RFID[3] = EEPROM_readByte(START_OF_RFID_USER + j*NUMBER_OF_BLOCK + 3);
		/* Read door*/
    RFID[4] = EEPROM_readByte(START_OF_RFID_USER + j*NUMBER_OF_BLOCK + 4);
		// Check day
		RFID[5] = EEPROM_readByte(START_OF_RFID_USER + j*NUMBER_OF_BLOCK + 5);
		//day = (uint8_t)RFID[5];
		// Check time
		hourFrom = EEPROM_readByte(START_OF_RFID_USER + j*NUMBER_OF_BLOCK + 6); //hourfrom
		minuteFrom = EEPROM_readByte(START_OF_RFID_USER + j*NUMBER_OF_BLOCK + 7); // minute from
		hourTo = EEPROM_readByte(START_OF_RFID_USER + j*NUMBER_OF_BLOCK + 8); // hour to
		minuteTo = EEPROM_readByte(START_OF_RFID_USER + j*NUMBER_OF_BLOCK + 9); // minute to
		
		//Check Day 
		if (((RFID[5] >> s[7]) & 0x01) != 1)
		{
			mode_temp = 0;
		}
		
		//Check Door
		if (((RFID[4] >> s[6]) & 0x01) != 1 )
		{
			mode_temp = 0;
		}
		RFID[4] = '\0';	// pass Day then delete Day from RFID[]
		//Remove 2 first is ID and Command
		*s++;
		*s++;
		if (strstr(s, RFID) == NULL) // if s does not contain RFID, Door
		{
			/*strcpy(outPutText,"OK+OPEN+");
			strcat(outPutText, ID);
			//for (i = 0; i < 9; i ++) s1[8+i] = ID[i];
			U_Print_Char(USART1, "OK+OPEN+");
			U_Print_Char(USART1, ID);
			flag = 1;
			break;*/
			
			mode_temp = 0;
		}
		// Check Time
		uint8_t hour = (timeNow.hour1 * 10) + timeNow.hour0;
		uint8_t minute = (timeNow.min1 * 10) + timeNow.min0;
		minute = (timeNow.min1 * 10) + timeNow.min0;
		//if ((hourFrom <= hour) && (hour <= hourTo) && (minuteFrom <= minute) && (minute <= minuteTo)) //time allow
		if (!((hourFrom <= hour) && (hour <= hourTo) && (minuteFrom <= minute) && (minute <= minuteTo)))
		{
			mode_temp = 0;
		}
		if (mode_temp == 1)
		{
			s[1] = OPEN;
			for (int i = 0; i < 12; i++)
				outPutText[i] = (uint8_t)s[i];
			return OPEN;
//			if (door == 1)
//			{
//				outPutText[0] = BOARDID;
//				s[1] = OPEN;
//				//strcat(outPutText, );
//				//strcat(outPutText,ID);
//				return OPEN;
//			}
//			else if (door == 2)
//			{
//				outPutText[0] = BOARDID;
//				s[1] = OPEN;
//				//trcat(outPutText, "AT+OPEN2");
//				//strcat(outPutText,ID);
//				return OPEN;
//			}
//			else if (door == 3)
//			{
//				outPutText[0] = BOARDID;
//				strcat(outPutText, "AT+OPEN3");
//				//strcat(outPutText,ID);
//				return OPEN;
//			}
//			else if (door == 4)
//			{
//				outPutText[0] = BOARDID;
//				strcat(outPutText, "AT+OPEN4");
//				//strcat(outPutText,ID);
//				return OPEN;
//			}
		}
	}
	/*
	quantity ++;
	for (int j = 0; j < 4; j++)
	{
		EEPROM_writeByte(START_OF_RFID_USER + (quantity)*NUMBER_OF_BLOCK + j, RFID[j]);
	}
	SetQuantity(quantity);
	*/
	//U_Print_Char(USART1,s);
	return NONE;
}

unsigned char *GetOutPutText() {return outPutText;}

void ID_DEL(char *ID_IN)
{
	int i,current;
	char ID[8];
	uint16_t quantity = GetQuantity();
	for (i = 0; i <= quantity; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			ID[j] = EEPROM_readByte(0x0010 + i*NUMBER_OF_BLOCK + j);
		}
		if (strcmp(ID, ID_IN) == 0) 
		{
			current = i;
			break;
		}
	}
	for (i = current; i < quantity; i++)
	{
		for (int j = 0; j < 23; j++)
		{
			uint8_t temp = EEPROM_readByte(0x0010 + (i+1)*NUMBER_OF_BLOCK +  j);
			EEPROM_writeByte(0x0010 + i*NUMBER_OF_BLOCK +j, temp);
			time = 0;
			while (time < 100);
		}
	}
}

int AddNewUser(unsigned char *s)
{
	uint16_t quantity = GetQuantity();
	//for (int i = 0; i < quantity; i++)
	{
		*s++;
		*s++;
		// 4 byte RFID
		EEPROM_writeByte(START_OF_RFID_USER + quantity*NUMBER_OF_BLOCK + 0, (uint8_t)*s);
		time = 0;
		TIM_SetCounter(TIM4, 0);
		while (time < 50) time = TIM_GetCounter(TIM4);
		//Delay(1);
		
		*s++;
		EEPROM_writeByte(START_OF_RFID_USER + quantity*NUMBER_OF_BLOCK + 1, (uint8_t)*s);
		time = 0;
		TIM_SetCounter(TIM4, 0);
		while (time < 50) time = TIM_GetCounter(TIM4);
		//Delay(1);
		
		*s++;
		EEPROM_writeByte(START_OF_RFID_USER + quantity*NUMBER_OF_BLOCK + 2, (uint8_t)*s);
		time = 0;
		TIM_SetCounter(TIM4, 0);
		while (time < 50) time = TIM_GetCounter(TIM4);
		//Delay(1);
		
		*s++;
		EEPROM_writeByte(START_OF_RFID_USER + quantity*NUMBER_OF_BLOCK + 3, (uint8_t)*s);
		time = 0;
		TIM_SetCounter(TIM4, 0);
		while (time < 50) time = TIM_GetCounter(TIM4);
		//Delay(1);
		
		*s++;
		// 1 byte door
		EEPROM_writeByte(START_OF_RFID_USER + quantity*NUMBER_OF_BLOCK + 4, (uint8_t)*s);
		time = 0;
		TIM_SetCounter(TIM4, 0);
		while (time < 50) time = TIM_GetCounter(TIM4);
		//Delay(1);
		
		*s++;
		//1 byte day
		EEPROM_writeByte(START_OF_RFID_USER + quantity*NUMBER_OF_BLOCK + 5, (uint8_t)*s);
		time = 0;
		TIM_SetCounter(TIM4, 0);
		while (time < 50) time = TIM_GetCounter(TIM4);
		//Delay(1);
		
		*s++;
		//4 byte hour
		EEPROM_writeByte(START_OF_RFID_USER + quantity*NUMBER_OF_BLOCK + 6, (uint8_t)*s);
		time = 0;
		TIM_SetCounter(TIM4, 0);
		while (time < 50) time = TIM_GetCounter(TIM4);
		//Delay(1);
		
		*s++;
		EEPROM_writeByte(START_OF_RFID_USER + quantity*NUMBER_OF_BLOCK + 7, (uint8_t)*s);
		time = 0;
		TIM_SetCounter(TIM4, 0);
		while (time < 50) time = TIM_GetCounter(TIM4);
		//Delay(1);
		
		*s++;
		EEPROM_writeByte(START_OF_RFID_USER + quantity*NUMBER_OF_BLOCK + 8, (uint8_t)*s);
		time = 0;
		TIM_SetCounter(TIM4, 0);
		while (time < 50) time = TIM_GetCounter(TIM4);
		//Delay(1);
		
		*s++;
		EEPROM_writeByte(START_OF_RFID_USER + quantity*NUMBER_OF_BLOCK + 9, (uint8_t)*s);
		time = 0;
		TIM_SetCounter(TIM4, 0);
		while (time < 50) time = TIM_GetCounter(TIM4);
		//Delay(1);
		
		// Set Quantity + 1
		quantity++;
		SetQuantity(quantity);
		
//		uint8_t t;
//		t = GetQuantity();
//		t = EEPROM_readByte(START_OF_RFID_USER + quantity*NUMBER_OF_BLOCK + 2);
//		t = EEPROM_readByte(START_OF_RFID_USER + quantity*NUMBER_OF_BLOCK + 5);
	}
	return 1;
}
