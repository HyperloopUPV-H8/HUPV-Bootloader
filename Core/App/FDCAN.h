/*
 * FDCAN.hpp
 *
 *  Created on: Mar 7, 2023
 *      Author: Pablo
 */

#ifndef APP_FDCAN_HPP_
#define APP_FDCAN_HPP_

#include "header.h"

#define FDCAN_DLC (FDCAN_DLC_BYTES_64)

#define FDCAN_MAX_QUEUE_SIZE ((uint8_t)16)

/****************************************************************************************
* Type definitions
****************************************************************************************/

typedef enum{
	FDCAN_OK = 0x00,
	FDCAN_EMPTY = 0x01,
	FDCAN_ERROR = 0xff,

}fdcan_error_t;

typedef struct {
	uint16_t identifier;
	uint8_t* data;

}fdcan_packet_t;

/****************************************************************************************
* Function prototypes
****************************************************************************************/

fdcan_error_t const fdcan_transmit(fdcan_packet_t* packet);

fdcan_error_t const fdcan_read(fdcan_packet_t* packet);

int const fdcan_test(void);


#endif /* APP_FDCAN_HPP_ */
