#ifndef _AT_COMMAND_H_
#define _AT_COMMAND_H_

#ifdef __cplusplus
extern "C" {
#endif
	
#define NONE 1
#define IDCHECK   2
#define OPEN1 3
#define OPEN2 4
#define OPEN3 5
#define OPEN4 6
#define IDREAD 7
#define IDADD 8
#define IDBOOL 9
#define IDDEL 10
#define CLEARALL 11
	
#include <string.h>
#include <stdlib.h>
	
extern char _read;
	
struct RFID
{
	char ADDR[8+1];
	char TIME_SEND[11+1];
	char DOOR[4+1];
};
	
char CheckAT(char *s);
char Check_ID(char *s);
char Check_Door(char *s);
	
#ifdef __cplusplus
}
#endif

#endif /* _AT_COMMAND_H_ */
