#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#ifdef __cplusplus
extern "C" {
#endif 
	
/******* Include *******/
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "25lc256.h"
#include "U_Print.h"
#include "string.h"
#include "AT_Command.h"
#include "main.h"
#include "DS1307.h"

	
#define NUMBER_OF_BLOCK 10
#define START_OF_RFID_USER 16
	
extern uint32_t time;
	
/******* Prototype of Function *******/	
void OpenDoor(int number);
void SysTick_Init(void);
void led_toggle(void);	
void CloseDoor(int number);
char * IDADD_READ_RFID(char *s);
char * IDADD_READ_TIME(char *s);
char * IDADD_READ_DOOR(char *s);
char CheckOpenDoor(char s[]);
void ID_DEL(char *ID_IN);
	
#ifdef __cplusplus
}
#endif

#endif /* _FUNCTION_H_ */
