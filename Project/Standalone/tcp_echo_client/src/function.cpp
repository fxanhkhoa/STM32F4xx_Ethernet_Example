#include "function.h"

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

char * IDADD_READ_RFID(char *s)
{
	char *temp = new char[8];
	temp[0] = s[9];
	temp[1] = s[10];
	temp[2] = s[11];
	temp[3] = s[12];
	temp[4] = s[13];
	temp[5] = s[14];
	temp[6] = s[15];
	temp[7] = s[16];
	return temp;
}

char * IDADD_READ_TIME(char *s)
{
	char *temp = new char[11];
	temp[0] = s[17];
	temp[1] = s[18];
	temp[2] = s[19];
	temp[3] = s[20];
	temp[4] = s[21];
	temp[5] = s[22];
	temp[6] = s[23];
	temp[7] = s[24];
	temp[8] = s[25];
	temp[9] = s[26];
	temp[10] = s[27];
	return temp;
}

char * IDADD_READ_DOOR(char *s)
{
	char *temp = new char[1];
	temp[0] = s[28];
	return temp;
}

char Check_Open_Door(char *s)
{
	//U_Print_Char(USART1,s);
	int i,flag = 0;
	char ID[10];
	for (i = 0; i < quantity; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			ID[j] = (char)EEPROM_readByte(0x0010 + i*NUMBER_OF_BLOCK + j);
		}
		
		ID[8] = (char)EEPROM_readByte(0x0010 + i*NUMBER_OF_BLOCK + 7 + 11 + 1);
		ID[9] = '\0';
		//U_Print_Char(USART1, ID);
		//U_Print_Char(USART1, "\n");
		
		if ((strstr(s, ID) != NULL)) 
		{
			/*strcpy(s1,"OK+OPEN+");
			for (i = 0; i < 9; i ++) s1[8+i] = ID[i];*/
			U_Print_Char(USART1, "OK+OPEN+");
			U_Print_Char(USART1, ID);
			flag = 1;
			break;
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

void ID_DEL(char *ID_IN)
{
	int i,current;
	char ID[8];
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
