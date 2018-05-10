
/**
 * @file eeprom.cpp
 * @author rcq
 * @version 1.1
 * @date 2017-07-4
 * @brief 
 */
#include "mbed.h"
#include "eeprom.h"
#include "stm32f0xx_hal_flash.h"

 
int FLASH_PageErase(int page_addr)
{
    FLASH_EraseInitTypeDef er = {
        .TypeErase = FLASH_TYPEERASE_PAGES,
        .PageAddress = page_addr,
        .NbPages = 1,
    };
    uint32_t fault_sec = 0;
    HAL_FLASH_Unlock();
    HAL_StatusTypeDef res = HAL_FLASHEx_Erase(&er, &fault_sec);
    HAL_FLASH_Lock();
	
    return res == HAL_OK ? 0 : -1;
}

/*
 * Must call this first to enable writing
 */
void enableEEPROMWriting() {
    HAL_StatusTypeDef status = HAL_FLASH_Unlock();
    FLASH_PageErase(EEPROM_START_ADDRESS); // required to re-write
    //add rcq
	HAL_FLASH_Unlock();
	CLEAR_BIT(FLASH->CR, FLASH_CR_PER); // Bug fix: bit PER has been set in Flash_PageErase(), must clear it here

}
 
void disableEEPROMWriting() {
    HAL_FLASH_Lock();
}
 
/*
 * Writing functions
 * Must call enableEEPROMWriting() first
 */
HAL_StatusTypeDef writeEEPROMHalfWord(uint32_t address, uint16_t data) {
    HAL_StatusTypeDef status;
    address = address + EEPROM_START_ADDRESS;
    
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, data);
    
    return status;
}
 
HAL_StatusTypeDef writeEEPROMWord(uint32_t address, uint32_t data) {
    HAL_StatusTypeDef status;
    address = address + EEPROM_START_ADDRESS;
    
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data);

    return status;
}
 
/*
 * Reading functions
 */
uint16_t readEEPROMHalfWord(uint32_t address) {
    uint16_t val = 0;
    address = address + EEPROM_START_ADDRESS;
    val = *(volatile uint16_t*)address;

    return val;
}
 
uint32_t readEEPROMWord(uint32_t address) {
    uint32_t val = 0;
    address = address + EEPROM_START_ADDRESS;
    //val = *(__IO uint32_t*)address;
    val = *(volatile uint16_t*)address;
    return val;
}

