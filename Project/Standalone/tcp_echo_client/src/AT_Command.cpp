#include "AT_Command.h"

/***** Variables *****/

char CheckAT(char *s)
{
	//strcpy(RFID_ID[0].ADDR,"B0562917");
	//strcpy(RFID_ID[0].ADDR,"B056291");
	if (strstr(s, "AT+IDCHECK") != NULL)
	{
		//_read = 0;
		return IDCHECK;
	}
	else if (strstr(s, "AT+OPEN1") != NULL)
	{
		return OPEN1;
	}
	else if (strstr(s, "AT+OPEN2") != NULL)
	{
		return OPEN2;
	}
	else if (strstr(s, "AT+OPEN3") != NULL)
	{
		return OPEN3;
	}
	else if (strstr(s, "AT+OPEN4") != NULL)
	{
		return OPEN4;
	}
	else if (strstr(s, "AT+IDREAD") != NULL)
	{
		return IDREAD;
	}
	else if (strstr(s, "AT+IDADD") != NULL)
	{
		return IDADD;
	}
	else if (strstr(s, "AT+IDDEL") != NULL)
	{
		return IDDEL;
	}
	else if (strstr(s, "AT+CLEARALL") != NULL)
	{
		return CLEARALL;
	}
	return NONE;
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
