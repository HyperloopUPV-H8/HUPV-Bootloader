/*
 * flash_test.h
 *
 *  Created on: Mar 7, 2023
 *      Author: predawnia
 */

#ifndef TESTS_FLASH_TEST_H_
#define TESTS_FLASH_TEST_H_

#include "../App/Bootloader.h"

void flash_test(){
	uint32_t data[SECTOR_SIZE_IN_WORDS];
	uint32_t i;
	for (i = 0; i < SECTOR_SIZE_IN_WORDS; ++i) {
		data[i] = 0xAABBCCDD;
	}

	HAL_GPIO_WritePin(LED_FLASH_GPIO_Port, LED_FLASH_Pin, GPIO_PIN_SET);
	if (FLASH_OK != flash_write(0x080a0000, data, SECTOR_SIZE_IN_WORDS)) {
		HAL_GPIO_WritePin(LED_FAULT_GPIO_Port, LED_FAULT_Pin, GPIO_PIN_SET);
	}

	for (i = 0; i < SECTOR_SIZE_IN_WORDS; ++i) {
		data[i] = 0x00000000;
	}

	if (FLASH_OK != flash_read(0x080c0000, data, SECTOR_SIZE_IN_WORDS)) {
		HAL_GPIO_WritePin(LED_FAULT_GPIO_Port, LED_FAULT_Pin, GPIO_PIN_SET);
	}

	if (FLASH_OK != flash_write(0x080c0000, data, SECTOR_SIZE_IN_WORDS)) {
		HAL_GPIO_WritePin(LED_FAULT_GPIO_Port, LED_FAULT_Pin, GPIO_PIN_SET);
	}

	if (FLASH_OK != flash_erase(FLASH_SECTOR_5, FLASH_SECTOR_5)) {
		HAL_GPIO_WritePin(LED_FAULT_GPIO_Port, LED_FAULT_Pin, GPIO_PIN_SET);

	}

	HAL_GPIO_WritePin(LED_FLASH_GPIO_Port, LED_FLASH_Pin, GPIO_PIN_RESET);
}

#endif /* TESTS_FLASH_TEST_H_ */
