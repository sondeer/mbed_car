#ifndef __EEPROM_FLASH_H
#define __EEPROM_FLASH_H
 
#include "mbed.h"
#include "stm32f0xx_hal_flash.h"
 

	#define EEPROM_START_ADDRESS        ((uint32_t)0x0801F800)

	void enableEEPROMWriting(); // Unlock and keep PER cleared
	void disableEEPROMWriting(); // Lock
	 
	// Write functions
	HAL_StatusTypeDef writeEEPROMHalfWord(uint32_t address, uint16_t data);
	HAL_StatusTypeDef writeEEPROMWord(uint32_t address, uint32_t data);
	 
	// Read functions
	uint16_t readEEPROMHalfWord(uint32_t address);
	uint32_t readEEPROMWord(uint32_t address);
 
#endif


