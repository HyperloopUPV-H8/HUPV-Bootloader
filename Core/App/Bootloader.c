/*
 * Bootloader.cpp
 *
 *  Created on: Mar 7, 2023
 *      Author: Pablo
 */

#include "Bootloader.h"

/****************************************************************************************
* Macro definitions
****************************************************************************************/
#define BOOTLOADER_ACK 	(0x79)
#define BOOTLOADER_NACK (0x1f)


extern TIM_HandleTypeDef htim23;

/****************************************************************************************
* Function prototypes
****************************************************************************************/
bootloader_error_t const __b_wait_until_fdcan_message_received(void);
void const __b_clean_fdcan_packet_data(fdcan_packet_t* packet);
void const __b_clean_fdcan_packet_all(fdcan_packet_t* packet);
bootloader_error_t const __b_wait_for_ack(bootloader_order_t order);
void const __b_send_ack(fdcan_packet_t* packet);
void const __b_send_nack(fdcan_packet_t* packet);
void const __b_data_copy_to_packet(fdcan_packet_t* packet, uint8_t* data);
void const __b_data_copy_from_packet(fdcan_packet_t* packet, uint8_t* data);
void const __b_get_version(fdcan_packet_t* packet);
void const __b_read_memory(fdcan_packet_t* packet);
void const __b_write_memory(fdcan_packet_t* packet);
void const __b_erase_memory(fdcan_packet_t* packet);
void const __b_go(fdcan_packet_t* packet);


void bootloader_start(){
	fdcan_packet_t packet;
	__b_clean_fdcan_packet_all(&packet);

	while(1){
		if (__b_wait_until_fdcan_message_received() != BOOTLOADER_OK) {
			continue;
		}

		fdcan_read(&packet);

		switch (packet.identifier) {
			case GET_VERSION_ORDER:
				__b_get_version(&packet);
				break;
			case READ_MEMORY_ORDER:
				__b_read_memory(&packet);
				break;
			case WRITE_MEMORY_ORDER:
				__b_write_memory(&packet);
				break;
			case ERASE_MEMORY_ORDER:
				__b_erase_memory(&packet);
				break;
			case GO_ORDER:
				__b_go(&packet);
				break;
			default:
				__b_send_nack(&packet);
				break;
		}

		__b_clean_fdcan_packet_all(&packet);
	}
}

void const __b_get_version(fdcan_packet_t* packet){
	__b_send_ack(packet);

	packet->data[0] = BOOTLOADER_VERSION;
	if (fdcan_transmit(packet) != FDCAN_OK) {
		__b_send_nack(packet);
		return;
	}

	__b_send_ack(packet);
}

void const __b_read_memory(fdcan_packet_t* packet){
	uint32_t buffer[SECTOR_SIZE_IN_32BITS_WORDS];
	sector_t sector;
	uint32_t address;
	uint16_t i, counter = 0;

	sector = packet->data[0];
	address = flash_get_sector_starting_address(sector);

	if (address == FLASH_SECTOR_ERROR || address == FLASH_PROTECTED_SECTOR) {
		__b_send_nack(packet);
		return;
	}

	if (flash_read(address, buffer, SECTOR_SIZE_IN_32BITS_WORDS) != FLASH_OK) {
		__b_send_nack(packet);
		return;
	}

	__b_send_ack(packet);

	if (__b_wait_for_ack(packet->identifier) != BOOTLOADER_OK) {
		__b_send_nack(packet);
		return;
	}

	for (i = 0; i < SECTOR_SIZE_IN_32BITS_WORDS; i +=16) {
		__b_data_copy_to_packet(packet, ((uint8_t*)&(buffer[i])));
		if (fdcan_transmit(packet) != FDCAN_OK) {
			__b_send_nack(packet);
			return;
		}
		if (counter >= BOOTLOADER_BLOCK_SIZE) {
			__b_send_ack(packet);
			if (__b_wait_for_ack(packet->identifier) != BOOTLOADER_OK) {
				__b_send_nack(packet);
				return;
			}
			counter = 0;
		}
		counter++;
		//HAL_Delay(1);
	}

	__b_send_ack(packet);
}

void const __b_write_memory(fdcan_packet_t* packet){
	uint32_t buffer[SECTOR_SIZE_IN_32BITS_WORDS];
	sector_t sector;
	uint32_t address;
	uint16_t i, counter = 1;

	sector = packet->data[0];
	address = flash_get_sector_starting_address(sector);

	if (address == FLASH_SECTOR_ERROR || address == FLASH_PROTECTED_SECTOR) {
		__b_send_nack(packet);
		return;
	}

	__b_send_ack(packet);

	if (__b_wait_for_ack(packet->identifier) != BOOTLOADER_OK) {
		__b_send_nack(packet);
		return;
	}

	for (i = 0; i < SECTOR_SIZE_IN_32BITS_WORDS; i +=16) {
		__b_wait_until_fdcan_message_received();
		if (fdcan_read(packet) != FDCAN_OK) {
			__b_send_nack(packet);
			return;
		}

		if (packet->identifier != WRITE_MEMORY_ORDER) {
			packet->identifier = WRITE_MEMORY_ORDER;
			__b_send_nack(packet);
			return;
		}

		__b_data_copy_from_packet(packet, ((uint8_t*)&(buffer[i])));

		if (counter >= BOOTLOADER_BLOCK_SIZE) {
			__b_send_ack(packet);
			if (__b_wait_for_ack(packet->identifier) != BOOTLOADER_OK) {
				__b_send_nack(packet);
				return;
			}
			counter = 1;
		}else{
			counter++;
		}

	}

	flash_write(address, buffer, SECTOR_SIZE_IN_32BITS_WORDS);

	__b_send_ack(packet);
}

void const __b_erase_memory(fdcan_packet_t* packet){
	sector_t sector1, sector2;

	sector1 = packet->data[0];
	sector2 = packet->data[1];

	if (sector1 > FLASH_MAX_SECTOR || sector2 > FLASH_MAX_SECTOR) {
		__b_send_nack(packet);
		return;
	}

	__b_send_ack(packet);


	if(flash_erase(sector1, sector2) != FLASH_OK){
		__b_send_nack(packet);
		return;
	}

	__b_send_ack(packet);
}

void const __b_go(fdcan_packet_t* packet){
	__b_send_nack(packet);
}

void const __b_data_copy_to_packet(fdcan_packet_t* packet, uint8_t* data){
	uint8_t i;

	for (i = 0; i < 64; ++i) {
		packet->data[i] = data[i];
	}
}

void const __b_data_copy_from_packet(fdcan_packet_t* packet, uint8_t* data){
	uint8_t i;

	for (i = 0; i < 64; ++i) {
		data[i] = packet->data[i];
	}
}

bootloader_error_t const __b_wait_for_ack(bootloader_order_t order){
	if (__b_wait_until_fdcan_message_received() != BOOTLOADER_OK) {
		return BOOTLOADER_ERROR;
	}

	fdcan_packet_t packet;
	fdcan_read(&packet);
	if (packet.identifier != order) {
		return BOOTLOADER_ERROR;
	}

	if (packet.data[0] != BOOTLOADER_ACK) {
		return BOOTLOADER_ERROR;
	}

	return BOOTLOADER_OK;
}

void const __b_send_ack(fdcan_packet_t* packet){
	__b_clean_fdcan_packet_data(packet);
	packet->data[0] = BOOTLOADER_ACK;
	fdcan_transmit(packet);
}

void const __b_send_nack(fdcan_packet_t* packet){
	__b_clean_fdcan_packet_data(packet);
	packet->data[0] = BOOTLOADER_NACK;
	fdcan_transmit(packet);
}

uint32_t const __b_get_addr_from_data(uint8_t* data){
	uint32_t address = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];

	return address;
}

void const __b_clean_fdcan_packet_data(fdcan_packet_t* packet){
	uint16_t i;

	for (i = 0; i < 64; ++i) {
		packet->data[i] = 0;
	}
}

void const __b_clean_fdcan_packet_all(fdcan_packet_t* packet){
	uint16_t i;

	for (i = 0; i < 64; ++i) {
		packet->data[i] = 0;
	}

	packet->identifier = 0;
}


bootloader_error_t const __b_wait_until_fdcan_message_received(void){
	htim23.Instance->CNT = 0;
	while(!fdcan_test()){
		if (htim23.Instance->CNT > BOOTLOADER_MAX_TIMEOUT * 10) {
			return BOOTLOADER_ERROR;
		}
	}

	return BOOTLOADER_OK;
}
