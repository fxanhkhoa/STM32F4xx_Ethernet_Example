#include "function.h"

//private variables
char outPutText[20];

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
char CheckOpenDoor(char *s)
{
	//U_Print_Char(USART1,s);
	int i,flag = 0;
	char ID[5];
	char *RFID = new char[5];
	uint16_t quantity = GetQuantity();
	outPutText[0] = '\0';
	for (i = 0; i < quantity; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ID[j] = (char)EEPROM_readByte(START_OF_RFID_USER + i*NUMBER_OF_BLOCK + j);
		}
		
		//ID[8] = (char)EEPROM_readByte(START_OF_RFID_USER + i*NUMBER_OF_BLOCK + 7 + 11 + 1);
		ID[4] = '\0';
		//U_Print_Char(USART1, ID);
		//U_Print_Char(USART1, "\n");
		
		if ((strstr(s, ID) != NULL)) // if ID in s
		{
			strcpy(outPutText,"OK+OPEN+");
			strcat(outPutText, ID);
			//for (i = 0; i < 9; i ++) s1[8+i] = ID[i];*/
			U_Print_Char(USART1, "OK+OPEN+");
			U_Print_Char(USART1, ID);
			// Check ID
			RFID = IDADD_READ_RFID(s);
			// Check door
			// Check time
			// Check day
			flag = 1;
			break;
		}
		else
		{
			quantity ++;
			for (int j = 0; j < 4; j++)
			{
				EEPROM_writeByte(START_OF_RFID_USER + (quantity)*NUMBER_OF_BLOCK + j, RFID[j]);
			}
		}
	}
	//U_Print_Char(USART1,s);
	if (flag == 1)
	{
		//DOOR[0] = s[18];
		if (ID[8] == '1') return OPEN1;
		else if (ID[8] == '2') return OPEN2;
		else if (ID[8] == '3')	return OPEN3;
		else if (ID[8] == '4')	return OPEN4;
	}
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
