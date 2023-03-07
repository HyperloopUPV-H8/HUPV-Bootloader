/*
 * Flash.cpp
 *
 *  Created on: Mar 7, 2023
 *      Author: Pablo
 */

#include "Flash.h"

/****************************************************************************************
* Function prototypes
****************************************************************************************/
sector_t const __flash_get_sector(const uint32_t address);
uint32_t const __flash_get_sector_starting_address(const sector_t sector);

flash_error const flash_read(uint32_t source_addr, uint32_t* result, uint32_t number_of_words){
	if (source_addr < FLASH_START_ADDRESS || source_addr > FLASH_CODE_END_ADDRESS) {
		return FLASH_PROTECTED_MEM;
	}

	HAL_FLASH_Unlock();
	uint32_t i;
	for (i = 0; i < number_of_words * FLASH_WORD; i+=4) {
		*result = *(__IO uint32_t *)(source_addr);
		source_addr += 4;
		result++;
	}
	HAL_FLASH_Lock();

	return FLASH_OK;
}

flash_error const flash_write(uint32_t dest_addr, uint32_t* data, uint32_t number_of_words){
	if (dest_addr < FLASH_SECTOR0_START_ADDRESS || dest_addr > FLASH_CODE_END_ADDRESS) {
		return FLASH_PROTECTED_MEM;
	}

	uint32_t index = 0;

	sector_t start_sector =__flash_get_sector(dest_addr);;
	uint32_t start_sector_addr = __flash_get_sector_starting_address(start_sector);

	uint32_t end_address = dest_addr + ((number_of_words * 4) - 4);
	uint32_t end_sector = __flash_get_sector(end_address);

	if (flash_erase(start_sector, end_sector) != FLASH_OK) {
		return FLASH_ERROR;
	}

	HAL_FLASH_Unlock();
	while(index < (uint32_t)SECTOR_SIZE_IN_WORDS){
		if (start_sector_addr > FLASH_CODE_END_ADDRESS) {
			return FLASH_PROTECTED_MEM;
		}

		data[index] = index;
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, start_sector_addr, (uint32_t)&data[index]) == HAL_OK) {
			start_sector_addr += 4 * FLASH_WORD;
			index += FLASH_WORD;
		}else{
			HAL_FLASH_Lock();
			return FLASH_ERROR;;
		}
	}
	HAL_FLASH_Lock();

	return FLASH_OK;
}

flash_error const flash_erase(const sector_t start_sector, const sector_t end_sector){
	if (start_sector == FLASH_PROTECTED_SECTOR || end_sector == FLASH_PROTECTED_SECTOR) {
		return FLASH_PROTECTED_MEM;
	}

	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t sectorError;

	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector        = start_sector;
	EraseInitStruct.Banks    	  = FLASH_BANK_1;
	EraseInitStruct.NbSectors     = (end_sector - start_sector) + 1;

	HAL_FLASH_Unlock();

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &sectorError) != HAL_OK)
	{
		HAL_FLASH_Lock();
		return FLASH_ERROR;
	}

	HAL_FLASH_Lock();

	return FLASH_OK;
}


sector_t const __flash_get_sector(const uint32_t address){
	sector_t sector = (sector_t)FLASH_SECTOR_0;

	/* BANK 1 */
	if((address >= 0x08000000) && (address < 0x08020000))
	{
		sector = FLASH_SECTOR_0;
	}
	else if((address >= 0x08020000) && (address < 0x08040000))
	{
		sector = FLASH_SECTOR_1;
	}
	else if((address >= 0x08040000) && (address < 0x08060000))
	{
		sector = FLASH_SECTOR_2;
	}
	else if((address >= 0x08060000) && (address < 0x08080000))
	{
		sector = FLASH_SECTOR_3;
	}
	else if((address >= 0x08080000) && (address < 0x080A0000))
	{
		sector = FLASH_SECTOR_4;
	}
	else if((address >= 0x080A0000) && (address < 0x080C0000))
	{
		sector = FLASH_SECTOR_5;
	}
	else if((address >= 0x080C0000) && (address < 0x080E0000))
	{
		sector = FLASH_SECTOR_6;
	}
	else if((address >= 0x080E0000) && (address < 0x08100000))
	{
		sector = FLASH_SECTOR_7;
	}

	return sector;
}

uint32_t const __flash_get_sector_starting_address(const sector_t sector){
	uint32_t address;
	switch ((uint32_t)sector) {
		case FLASH_SECTOR_0:
			address = FLASH_SECTOR0_START_ADDRESS;
			break;
		case FLASH_SECTOR_1:
			address = FLASH_SECTOR1_START_ADDRESS;
			break;
		case FLASH_SECTOR_2:
			address = FLASH_SECTOR2_START_ADDRESS;
			break;
		case FLASH_SECTOR_3:
			address = FLASH_SECTOR3_START_ADDRESS;
			break;
		case FLASH_SECTOR_4:
			address = FLASH_SECTOR4_START_ADDRESS;
			break;
		case FLASH_SECTOR_5:
			address = FLASH_SECTOR5_START_ADDRESS;
			break;
		case FLASH_SECTOR_6:
			address = FLASH_SECTOR6_START_ADDRESS;
			break;
		case FLASH_SECTOR_7:
			address = FLASH_SECTOR7_START_ADDRESS;
			break;
		default:
			address = FLASH_SECTOR0_START_ADDRESS;
			break;
	}

	return address;
}
