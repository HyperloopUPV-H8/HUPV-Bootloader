/*
 * fdcan_test.h
 *
 *  Created on: 8 mar. 2023
 *      Author: pablo
 */

#ifndef TESTS_FDCAN_TEST_H_
#define TESTS_FDCAN_TEST_H_

#include "../App/Bootloader.h"
uint8_t data[64];

void const fdcan_test(void){
	int i;
	uint8_t d = 0xA;
	for (i = 0; i < 64; ++i) {
		data[i] = d++;
	}

	fdcan_packet_t p = { 69, data};


	while (1)
	{
		fdcan_transmit(&p);


		HAL_Delay(100);
		if (fdcan_test()) {
			fdcan_read(&p);
		}
	}
}


#endif /* TESTS_FDCAN_TEST_H_ */
