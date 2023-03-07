/*
 * FDCAN.cpp
 *
 *  Created on: Mar 7, 2023
 *      Author: Pablo
 */

#include "FDCAN.h"

extern FDCAN_HandleTypeDef hfdcan1;

fdcan_instance_t fdcan_instance = {};

fdcan_error_t const fdcan_start(void){
	fdcan_instance.hfdcan = &hfdcan1;
	fdcan_instance.rx_location = FDCAN_RX_FIFO0;

	FDCAN_TxHeaderTypeDef* header = &fdcan_instance.tx_header;

	header->FDFormat = FDCAN_FD_CAN;
	header->DataLength = FDCAN_DLC_BYTES_64;
	header->TxFrameType = FDCAN_DATA_FRAME;
	header->BitRateSwitch = FDCAN_BRS_ON;
	header->ErrorStateIndicator = FDCAN_ESI_PASSIVE;
	header->FDFormat = FDCAN_FRAME_FD_BRS;
	header->IdType = FDCAN_STANDARD_ID;
	header->TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	header->MessageMarker = 0;
	header->Identifier = 0x0;

	if(HAL_FDCAN_Start(fdcan_instance.hfdcan) != HAL_OK){
		return FDCAN_ERROR;
	}

	 if(HAL_FDCAN_ActivateNotification(fdcan_instance.hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK){
		return FDCAN_ERROR;
	}

}

fdcan_error_t const fdcan_transmit(fdcan_packet_t* data){
	fdcan_instance.tx_header.Identifier = data->identifier;

	if (HAL_FDCAN_AddMessageToTxFifoQ(fdcan_instance.hfdcan, &fdcan_instance.tx_header, data->data) != HAL_OK) {
		return FDCAN_ERROR;
	}

	return FDCAN_OK;
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs){


}

fdcan_error_t const fdcan_read(fdcan_packet_t* data){
	if(!fdcan_test) {
		return FDCAN_EMPTY;
	}

	FDCAN_RxHeaderTypeDef header_buffer = {};
	HAL_FDCAN_GetRxMessage(fdcan_instance.hfdcan, fdcan_instance.rx_location, &header_buffer, data->data);

	data->identifier = header_buffer.Identifier;
	return FDCAN_OK;
}

int const fdcan_test(void){
	return HAL_FDCAN_IsRxBufferMessageAvailable(fdcan_instance.hfdcan, fdcan_instance.rx_location);
}
