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
	uint8_t data[64];
	uint16_t identifier;

}fdcan_packet_t;

typedef struct{
    FDCAN_HandleTypeDef* hfdcan;
    FDCAN_TxHeaderTypeDef tx_header;
    uint32_t rx_location;

}fdcan_instance_t;
/****************************************************************************************
* Function prototypes
****************************************************************************************/

fdcan_error_t const fdcan_start(void);

fdcan_error_t const fdcan_transmit(fdcan_packet_t* data);

fdcan_error_t const fdcan_read(fdcan_packet_t* data);

int const fdcan_test(void);


#endif /* APP_FDCAN_HPP_ */
