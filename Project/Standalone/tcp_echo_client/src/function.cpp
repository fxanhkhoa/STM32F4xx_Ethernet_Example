#include "function.h"

//private variables
char outPutText[30];
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
char CheckOpenDoor(char s[])
{
	//U_Print_Char(USART1,s);
	char mode_temp = NONE;
	int i,flag = 0;
	uint16_t j;
	char ID[5];
	char *RFID = new char[5];
	char door;
	char day;
	char hourFrom, hourTo;
	char minuteFrom, minuteTo;
	uint16_t quantity = GetQuantity();
	outPutText[0] = '\0';
	for (i = 0; i < quantity; i++)
	{
		mode_temp = 1; // true state
		timeNow = DS1307ReadTime(); // Get current time
		// Check ID
		for (j = 0; j < 4; j++)
		{
			ID[j] = (char)EEPROM_readByte(START_OF_RFID_USER + i*NUMBER_OF_BLOCK + j);
		}
		//ID[4] = '\0';
		//U_Print_Char(USART1, ID);
		//U_Print_Char(USART1, "\n");
		/* Check door*/
		door = (char)EEPROM_readByte(START_OF_RFID_USER + i*NUMBER_OF_BLOCK + 4);
		// Check day
		day = (char)EEPROM_readByte(START_OF_RFID_USER + i*NUMBER_OF_BLOCK + 5);
		// Check time
		hourFrom = (char) EEPROM_readByte(START_OF_RFID_USER + i*NUMBER_OF_BLOCK + 0x06);
		minuteFrom = (char) EEPROM_readByte(START_OF_RFID_USER + i*NUMBER_OF_BLOCK + 7);
		hourTo = (char) EEPROM_readByte(START_OF_RFID_USER + i*NUMBER_OF_BLOCK + 8);
		minuteTo = (char) EEPROM_readByte(START_OF_RFID_USER + i*NUMBER_OF_BLOCK + 9);
		
		if ((strstr(s, ID) == NULL)) // if ID not in s
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
		// Check Door
		if (door != s[14])
		{
			mode_temp = 0;
		}
		// Check Day
		if (((day >> s[15]) & (0x01)) == 1) // position is 16 
		{
			mode_temp = 0;
		}
		// Check Time
		uint8_t hour = (timeNow.hour1 * 10) + timeNow.hour0;
		uint8_t minute = (timeNow.min1 * 10) + timeNow.min0;
		//if ((hourFrom <= hour) && (hour <= hourTo) && (minuteFrom <= minute) && (minute <= minuteTo)) //time allow
		if (!((hourFrom <= hour) && (hour <= hourTo) && (minuteFrom <= minute) && (minute <= minuteTo)))
		{
			mode_temp = 0;
		}
		if (mode_temp == 1)
		{
			outPutText[0] = '\0';
			if (door == 1)
			{
				outPutText[0] = BOARDID;
				strcat(outPutText, "AT+OPEN1");
				strcat(outPutText,ID);
				return OPEN1;
			}
			else if (door == 2)
			{
				outPutText[0] = BOARDID;
				strcat(outPutText, "AT+OPEN2");
				strcat(outPutText,ID);
				return OPEN2;
			}
			else if (door == 3)
			{
				outPutText[0] = BOARDID;
				strcat(outPutText, "AT+OPEN3");
				strcat(outPutText,ID);
				return OPEN3;
			}
			else if (door == 4)
			{
				outPutText[0] = BOARDID;
				strcat(outPutText, "AT+OPEN4");
				strcat(outPutText,ID);
				return OPEN4;
			}
		}
	}
	quantity ++;
	for (int j = 0; j < 4; j++)
	{
		EEPROM_writeByte(START_OF_RFID_USER + (quantity)*NUMBER_OF_BLOCK + j, RFID[j]);
	}
	SetQuantity(quantity);
	//U_Print_Char(USART1,s);
	return NONE;
}

char *GetOutPutText() {return outPutText;}

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
