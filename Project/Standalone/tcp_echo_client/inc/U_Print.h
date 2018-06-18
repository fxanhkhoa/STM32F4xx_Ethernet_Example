#ifndef _U_Print_H_
#define _U_Print_H_

#ifdef __cplusplus
extern "C" {
#endif 

/* Includes */
#include "stm32f4xx.h"
#include <stdlib.h>
#include <stdio.h>
	
void U_Print(USART_TypeDef * USARTx, uint8_t a);
void U_Print_Char(USART_TypeDef * USARTx, char *s);
void U_Print_float(USART_TypeDef * USARTx, float a);

#ifdef __cplusplus
}
#endif

#endif /* _U_Print_H_ */
