/**
 * @file rd-03d_high.c
 * @brief Implementation of the high-level API for the RD-03D radar module.
 *
 * This file contains the logic for parsing raw radar frames, converting
 * sign-magnitude bytes into physical metrics, and safely dispatching
 * the processed data to FreeRTOS queues.
 */

#include "rd_handle.h"
#include "rd-03d_high.h"
#include "rd-03d_low.h"

/* --- Protocol Constants --- */
#define SIZE_TARGET_DATA 8
#define HIGH_15_BIT_ENABLE 0x8000
#define HIGH_15_BIT_DISABLE 0x7fff

/** @brief Reference array used to quickly check if a target slot is empty via memcmp */
static const uint8_t empty_target[SIZE_TARGET_DATA] = { 0 };

static rd_high_target_status_t compute_metrics(rd_target_t *target, uint8_t[]);

static int16_t compute_signed_data(uint8_t low_byte, uint8_t high_byte);

static uint16_t compute_unsigned_data(uint8_t low_byte, uint8_t high_byte);

rd_handle_init_t* createEntity(uart_port_t uart_num, int tx_io_num,
		int rx_io_num, QueueHandle_t uart_queue, QueueHandle_t data_queue) {

	rd_handle_init_t *handle = (rd_handle_init_t*) malloc(
			sizeof(struct rd_handle_init_s));

	if (NULL != handle) {
		handle->uart_num = uart_num;
		handle->tx_io_num = tx_io_num;
		handle->rx_io_num = rx_io_num;
		handle->uart_queue = uart_queue;
		handle->data_queue = data_queue;
		handle->initialize_status = RD_STATUS_NOT_INITIALIZED;
		handle->size_buffer = 24;
	}

	return handle;
}

rd_high_api_status_t rd_init(rd_handle_init_t *rd_handle) {
	rd_high_api_status_t statusCode = RD_STATUS_OK;

	if (NULL == rd_handle) {
		statusCode = RD_STATUS_INVALID_PARAMETERS;
	}

	if (RD_STATUS_OK == statusCode) {
		if (DEVICE_STATUS_OK != device_init(rd_handle)) {
			statusCode = RD_STATUS_NOT_INITIALIZED;
		}
	}

	if (RD_STATUS_OK == statusCode) {
		rd_handle->initialize_status = RD_STATUS_INITIALIZED;
	}

	return statusCode;
}

rd_high_api_status_t rd_read(rd_handle_init_t *rd_handle) {
	rd_high_api_status_t statusCode = RD_STATUS_OK;

	if (NULL == rd_handle) {
		statusCode = RD_STATUS_INVALID_PARAMETERS;
	}

	if (RD_STATUS_OK == statusCode
			&& RD_STATUS_INITIALIZED != rd_handle->initialize_status) {
		statusCode = RD_STATUS_NOT_INITIALIZED;
	}

	if (RD_STATUS_OK == statusCode) {
		if (DEVICE_STATUS_OK == device_read(rd_handle)) {
			rd_data_t current_frame = { 0 };
			uint8_t active_targets = 0;
			if (RD_TARGET_CONFIRMED
					== compute_metrics(&current_frame.targets[0],
							&rd_handle->data_buffer[0])) {
				active_targets++;
				current_frame.targets[0].id = 1;
			}
			if (RD_TARGET_CONFIRMED
					== compute_metrics(&current_frame.targets[1],
							&rd_handle->data_buffer[8])) {
				active_targets++;
				current_frame.targets[1].id = 2;
			}
			if (RD_TARGET_CONFIRMED
					== compute_metrics(&current_frame.targets[2],
							&rd_handle->data_buffer[16])) {
				active_targets++;
				current_frame.targets[2].id = 3;
			}

			current_frame.active_count = active_targets;

			if (xQueueSend(rd_handle->data_queue, &current_frame, 0) != pdTRUE) {
			        statusCode = RD_STATUS_READ_ERROR;
			} else {
				statusCode = RD_STATUS_OK;
			}

		} else {
			statusCode = RD_STATUS_READ_ERROR;
		}
	}

	return statusCode;
}

/**
 * @brief Extracts and computes metrics for a single target from a raw byte array.
 *
 * @param target Pointer to the target structure to populate.
 * @param target_data Array containing 8 bytes of raw target data.
 * @return rd_high_target_status_t RD_TARGET_CONFIRMED if a valid target is parsed, RD_NO_TARGET if empty.
 */
static rd_high_target_status_t compute_metrics(rd_target_t *target,
		uint8_t target_data[]) {
	rd_high_target_status_t statusCode = RD_TARGET_CONFIRMED;

	if (memcmp(target_data, empty_target, SIZE_TARGET_DATA) == 0) {
		statusCode = RD_NO_TARGET;
	} else {
		statusCode = RD_TARGET_CONFIRMED;
	}

	if (statusCode == RD_TARGET_CONFIRMED) {
		target->x = compute_signed_data(target_data[0], target_data[1]);
		target->y = compute_signed_data(target_data[2], target_data[3]);
		target->velocity = compute_signed_data(target_data[4], target_data[5]);
		target->distance = compute_unsigned_data(target_data[6],
				target_data[7]);
	}

	return statusCode;
}

/**
 * @brief Converts two bytes into a signed 16-bit integer based on radar protocol rules.
 *
 * The RD-03D radar uses a custom sign-magnitude format where the 15th bit
 * indicates the sign (1 = Positive, 0 = Negative).
 *
 * @param low_byte The lower 8 bits of the value.
 * @param high_byte The upper 8 bits of the value (including the sign bit).
 * @return int16_t The computed signed integer value.
 */
static int16_t compute_signed_data(uint8_t low_byte, uint8_t high_byte) {
	uint16_t combined_data = (high_byte << 8) | low_byte;

	bool sign = (combined_data & HIGH_15_BIT_ENABLE) != 0;

	uint16_t abs_value = combined_data & HIGH_15_BIT_DISABLE;

	return sign ? (int16_t) abs_value : -(int16_t) abs_value;
}

/**
 * @brief Converts two bytes into an unsigned 16-bit integer.
 *
 * @param low_byte The lower 8 bits of the value.
 * @param high_byte The upper 8 bits of the value.
 * @return uint16_t The computed unsigned integer value.
 */
static uint16_t compute_unsigned_data(uint8_t low_byte, uint8_t high_byte) {
	uint16_t combined_data = (high_byte << 8) | low_byte;
	return combined_data;
}
