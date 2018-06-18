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

	
#define NUMBER_OF_BLOCK 20
	
extern uint8_t quantity;	
extern uint32_t time;
extern char _read;
extern char s1[40];
extern uint8_t pos1;
	
/******* Prototype of Function *******/	
void OpenDoor(int number);
void SysTick_Init(void);
void led_toggle(void);	
void CloseDoor(int number);
char * IDADD_READ_RFID(char *s);
char * IDADD_READ_TIME(char *s);
char * IDADD_READ_DOOR(char *s);
char Check_Open_Door(char *s);
void ID_DEL(char *ID_IN);
	
#ifdef __cplusplus
}
#endif

#endif /* _FUNCTION_H_ */
