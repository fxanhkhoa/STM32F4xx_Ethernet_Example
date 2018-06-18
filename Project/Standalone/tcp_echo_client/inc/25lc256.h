#ifndef _25LC256_H_
#define _25LC256_H_

#ifdef __cplusplus
extern "C" {
#endif 
	
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"
	
#define SLAVE_SELECT GPIO_ResetBits(GPIOA, GPIO_Pin_15)
#define SLAVE_DESELECT GPIO_SetBits(GPIOA, GPIO_Pin_15)
	
	
// Instruction Set -- from data sheet
#define EEPROM_READ      0x03                     /* read memory 0b00000011*/
#define EEPROM_WRITE     0x02                 /* write to memory 0b00000010*/

#define EEPROM_WRDI      0x04                   /* write disable 0b00000100*/
#define EEPROM_WREN      0x06                    /* write enable 0b00000110*/

#define EEPROM_RDSR      0x05            /* read status register 0b00000101*/
#define EEPROM_WRSR      0x01           /* write status register 0b00000001*/

// EEPROM Status Register Bits -- from data sheet
// Use these to parse status register
#define EEPROM_WRITE_IN_PROGRESS    0
#define EEPROM_WRITE_ENABLE_LATCH   1
#define EEPROM_BLOCK_PROTECT_0      2
#define EEPROM_BLOCK_PROTECT_1      3

#define EEPROM_BYTES_PER_PAGE       64
#define EEPROM_BYTES_MAX            0x7FFF

#define SPI_SE SPI3	

uint8_t SPI_ReadWrite(uint8_t data);
void SPI_Write_Reg(uint8_t addr, uint8_t value);
uint8_t SPI_Read_Reg(uint8_t addr);
void EEPROM_writeEnable(void);
uint8_t EEPROM_readStatus(void); /* reads the EEPROM status register */
void EEPROM_send16BitAddress(uint16_t address);/* splits 16-bit address into 2 bytes, sends both */
uint8_t EEPROM_readByte(uint16_t address); /* gets a byte from a given memory location */                        
uint16_t EEPROM_readWord(uint16_t address);/* gets two bytes from a given memory location */                          
void EEPROM_writeByte(uint16_t address, uint8_t byte); /* writes a byte to a given memory location */                        
void EEPROM_writeWord(uint16_t address, uint16_t word);  /* gets two bytes to a given memory location */                                 
void EEPROM_clearAll(void);/* sets every byte in memory to zero */
	
#ifdef __cplusplus
}
#endif

#endif /* _25lc256_H_ */
