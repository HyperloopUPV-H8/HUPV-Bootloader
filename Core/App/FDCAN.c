/*
 * FDCAN.cpp
 *
 *  Created on: Mar 7, 2023
 *      Author: Pablo
 */

#include "FDCAN.h"

extern FDCAN_HandleTypeDef hfdcan1;
FDCAN_TxHeaderTypeDef tx_header;
FDCAN_RxHeaderTypeDef rx_header_buffer;

fdcan_error_t const fdcan_transmit(fdcan_packet_t* packet){

		tx_header.Identifier = (uint32_t)packet->identifier;
		tx_header.IdType = FDCAN_EXTENDED_ID;
		tx_header.TxFrameType = FDCAN_DATA_FRAME;
		tx_header.DataLength = FDCAN_DLC_BYTES_64;
		tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
		tx_header.BitRateSwitch = FDCAN_BRS_OFF;
		tx_header.FDFormat = FDCAN_FD_CAN;
		tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
		tx_header.MessageMarker = 0;

	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &tx_header, packet->data) != HAL_OK) {
		return FDCAN_ERROR;
	}

	return FDCAN_OK;
}

fdcan_error_t const fdcan_read(fdcan_packet_t* packet){
	if(!fdcan_test()) {
		return FDCAN_EMPTY;
	}

	HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &rx_header_buffer, packet->data);

	packet->identifier = rx_header_buffer.Identifier;
	return FDCAN_OK;
}

int const fdcan_test(void){
	return !((hfdcan1.Instance->RXF0S & FDCAN_RXF0S_F0FL) == 0U);
}
