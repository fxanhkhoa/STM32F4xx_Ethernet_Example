#ifndef _AT_COMMAND_H_
#define _AT_COMMAND_H_

#ifdef __cplusplus
extern "C" {
#endif

#define NONE 0
#define IDCHECK 1
#define IDADD	2
#define IDDEL 3
#define SETDS1307 4
#define DS1307 5
#define OPEN 6
#define CLEARALL 7
#define STOREOK 8
#define GETINFO 9
#define SETINFO 10
	
#include <string.h>
#include <stdlib.h>
#include "stm32f4xx.h"
	
extern char _read;
	
struct RFID
{
	char ADDR[8+1];
	char TIME_SEND[11+1];
	char DOOR[4+1];
};
	
uint8_t CheckAT(char *s);
char Check_ID(char *s);
char Check_Door(char *s);
	
#ifdef __cplusplus
}
#endif

#endif /* _AT_COMMAND_H_ */
