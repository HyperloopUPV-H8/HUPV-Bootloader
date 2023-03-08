/*
 * Bootloader.hpp
 *
 *  Created on: Mar 7, 2023
 *      Author: Pablo
 */

#ifndef APP_BOOTLOADER_HPP_
#define APP_BOOTLOADER_HPP_

#include "header.h"
#include "Flash.h"
#include "FDCAN.h"

#define BOOTLOADER_VERSION (0x01)

/****************************************************************************************
* Type definitions
****************************************************************************************/
typedef enum{
	GET_VERSION_ORDER = 0x50,
	READ_MEMORY_ORDER = 0x40,
	WRITE_MEMORY_ORDER = 0x30,
	ERASE_MEMORY_ORDER = 0x20,
	GO_ORDER = 0x10
}bootloader_order_t;

/****************************************************************************************
* Function prototypes
****************************************************************************************/
void const bootloader_start(void);

#endif /* APP_BOOTLOADER_HPP_ */
