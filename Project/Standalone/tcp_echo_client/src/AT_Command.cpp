#include "AT_Command.h"

/***** Variables *****/

/*
Name: CheckAT

Description: Use this function to check AT command

Input: buffer for checking

Output: Number of Mode defined in AT_Command.h
*/
uint8_t CheckAT(char *s)
{
	//strcpy(RFID_ID[0].ADDR,"B0562917");
	//strcpy(RFID_ID[0].ADDR,"B056291");
	switch (s[1])
	{
		
	};
	return IDADD;
}

char Check_ID(char *s)
{
	if (strstr(s, "B0562917") != NULL) 
	{
		return 1;
	}
	return 0;
}

char Check_Door(char *s)
{
	if (s[5] == '1')
	{
		return 1;
	}
	else if (s[5] == '2')
	{
		return 2;
	}
	else if (s[5] == '3')
	{
		return 3;
	}
	else if (s[5] == '4')
	{
		return 4;
	}
	return 0;
}
