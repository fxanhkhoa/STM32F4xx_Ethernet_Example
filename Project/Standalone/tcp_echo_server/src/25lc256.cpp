#include "25lc256.h"

uint8_t SPI_ReadWrite(uint8_t data)
{
	/* Loop while DR register in not empty */
    while (!SPI_I2S_GetFlagStatus(SPI_SE, SPI_I2S_FLAG_TXE));
    
    /* Send data byte */
    SPI_I2S_SendData(SPI_SE, data);
    
    /* Wait for received byte */
    while (!SPI_I2S_GetFlagStatus(SPI_SE, SPI_I2S_FLAG_RXNE));
    
    /* Return byte received from bus */
    return SPI_I2S_ReceiveData(SPI_SE);

}

void SPI_Write_Reg(uint8_t addr, uint8_t value)
{
		//Pull CS low
    SLAVE_SELECT;
    
    // Send address ( address shift 1 and bit 7 = 0 for mode write)
    SPI_ReadWrite((addr << 1) | 0x7E);
    
    //Send value
    SPI_ReadWrite(value);
	
		SLAVE_DESELECT;

}

uint8_t SPI_Read_Reg(uint8_t addr)
{
    //uint8_t status;
    uint8_t val;
    
    // Pull CS low
		SLAVE_SELECT;
    
    // Send address , clear bit 1,7 and set bit 1 for read mode
    SPI_ReadWrite(((addr << 1) & 0x7E) | 0x80);
    
    // Read value at register 0x00
    val = SPI_ReadWrite(0x00);
    
    //Pull CS HIGH
    SLAVE_DESELECT;
    
    return val;
}

void EEPROM_writeEnable(void) 
{
	SLAVE_SELECT;
  SPI_ReadWrite(EEPROM_WREN);
	SLAVE_DESELECT;
}

uint8_t EEPROM_readStatus(void) 
{
	uint8_t temp;
  SLAVE_SELECT;
  temp = SPI_ReadWrite(EEPROM_RDSR);
  temp = SPI_ReadWrite(0);                            /* clock out eight bits */
  SLAVE_DESELECT;
  return (temp);                                  /* return the result */
}


void EEPROM_send16BitAddress(uint16_t address) {
  SPI_ReadWrite((uint8_t) (address >> 8));    /* most significant byte */
  SPI_ReadWrite((uint8_t) address);          /* least significant byte */
}

uint8_t EEPROM_readByte(uint16_t address) 
{
  SLAVE_SELECT;
  uint8_t temp = SPI_ReadWrite(EEPROM_READ);
  EEPROM_send16BitAddress(address);
  temp = SPI_ReadWrite(0);
  SLAVE_DESELECT;
  return (temp);
}

uint16_t EEPROM_readWord(uint16_t address) {
  uint16_t eepromWord;
	uint8_t temp;
  SLAVE_SELECT;
  temp = SPI_ReadWrite(EEPROM_READ);
  EEPROM_send16BitAddress(address);
  temp = SPI_ReadWrite(0);
  eepromWord = temp;
  eepromWord = (eepromWord << 8);                      /* most-sig bit */
  temp = SPI_ReadWrite(0);
  eepromWord += temp;                                 /* least-sig bit */
  SLAVE_DESELECT;
  return (eepromWord);
}

void EEPROM_writeByte(uint16_t address, uint8_t byte) \
{
  EEPROM_writeEnable();
  SLAVE_SELECT;
  SPI_ReadWrite(EEPROM_WRITE);
  EEPROM_send16BitAddress(address);
  SPI_ReadWrite(byte);
  SLAVE_DESELECT;
  while (EEPROM_readStatus() & (EEPROM_WRITE_IN_PROGRESS));
}

void EEPROM_writeWord(uint16_t address, uint16_t word) 
{
  EEPROM_writeEnable();
  SLAVE_SELECT;
  SPI_ReadWrite(EEPROM_WRITE);
  EEPROM_send16BitAddress(address);
  SPI_ReadWrite((uint8_t) (word >> 8));
  SPI_ReadWrite((uint8_t) word);
  SLAVE_DESELECT;
  while (EEPROM_readStatus() & (EEPROM_WRITE_IN_PROGRESS));
}

void EEPROM_clearAll(void) {
  uint8_t i;
  uint16_t pageAddress = 0;
  while (pageAddress < EEPROM_BYTES_MAX) 
	{
    EEPROM_writeEnable();
    SLAVE_SELECT;
    SPI_ReadWrite(EEPROM_WRITE);
    EEPROM_send16BitAddress(pageAddress);
    for (i = 0; i < EEPROM_BYTES_PER_PAGE; i++) {
      SPI_ReadWrite(0);
    }
    SLAVE_DESELECT;
    pageAddress += EEPROM_BYTES_PER_PAGE;
    while (EEPROM_readStatus() & (EEPROM_WRITE_IN_PROGRESS));
  }
}
