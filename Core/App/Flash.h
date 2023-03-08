/*
 * Flash.hpp
 *
 *  Created on: Mar 7, 2023
 *      Author: Pablo
 */

#ifndef APP_FLASH_H_
#define APP_FLASH_H_

/****************************************************************************************
* Include files
****************************************************************************************/
#include "header.h"

/****************************************************************************************
* Macro definitions
****************************************************************************************/
#define FLASH_WORD_SIZE				((uint8_t) 32U)
#define FLASH_32BITS_WORLD			((uint8_t) 4U)

#define SECTOR_SIZE_IN_32BITS_WORDS ((uint16_t)32768U)
#define SECTOR_SIZE_IN_BYTES		((uint32_t)131072U)
#define SECTOR_SIZE_IN_KILOBYTES	((uint8_t) 128U)

#define FLASH_SECTOR0_START_ADDRESS	((uint32_t)0x08000000U)
#define FLASH_SECTOR0_END_ADDRESS	((uint32_t)0x0801FFFFU)

#define FLASH_SECTOR1_START_ADDRESS ((uint32_t)0x08020000U)
#define FLASH_SECTOR1_END_ADDRESS	((uint32_t)0x0803FFFFU)

#define FLASH_SECTOR2_START_ADDRESS ((uint32_t)0x08040000U)
#define FLASH_SECTOR2_END_ADDRESS	((uint32_t)0x0805FFFFU)

#define FLASH_SECTOR3_START_ADDRESS ((uint32_t)0x08060000U)
#define FLASH_SECTOR3_END_ADDRESS	((uint32_t)0x0807FFFFU)

#define FLASH_SECTOR4_START_ADDRESS ((uint32_t)0x08080000U)
#define FLASH_SECTOR4_END_ADDRESS	((uint32_t)0x0809FFFFU)

#define FLASH_SECTOR5_START_ADDRESS ((uint32_t)0x080A0000U)
#define FLASH_SECTOR5_END_ADDRESS	((uint32_t)0x080BFFFFU)

#define FLASH_SECTOR6_START_ADDRESS ((uint32_t)0x080C0000U)
#define FLASH_SECTOR6_END_ADDRESS	((uint32_t)0x080DFFFFU)

#define FLASH_SECTOR7_START_ADDRESS ((uint32_t)0x080E0000U)
#define FLASH_SECTOR7_END_ADDRESS	((uint32_t)0x080FFFFFU)

#define FLASH_SECTOR_ERROR			((uint32_t)0xFFFFFFFFU)

#define FLASH_START_ADDRESS 		FLASH_SECTOR0_START_ADDRESS
#define FLASH_END_ADDRESS 			FLASH_SECTOR7_END_ADDRESS

#define FLASH_CODE_END_ADDRESS 		FLASH_SECTOR6_END_ADDRESS

#define FLASH_MAX_SECTOR 			FLASH_SECTOR_7

#define FLASH_PROTECTED_SECTOR      FLASH_SECTOR_7


/****************************************************************************************
* Type definitions
****************************************************************************************/
typedef enum{
	FLASH_OK = 0x00,
	FLASH_PROTECTED_MEM = 0x01,
	FLASH_ERROR = 0xff,
}flash_error;

typedef uint32_t sector_t;

/****************************************************************************************
* Function prototypes
****************************************************************************************/
flash_error const flash_read(uint32_t source_addr, uint32_t* result, uint32_t number_of_words);

flash_error const flash_write(uint32_t dest_addr, uint32_t* data, uint32_t number_of_words);

flash_error const flash_erase(const sector_t start_sector, const sector_t end_sector);

uint32_t const flash_get_sector_starting_address(const sector_t sector);

#endif /* APP_FLASH_H_ */
