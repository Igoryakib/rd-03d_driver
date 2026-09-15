/**
 * @file rd-03d_low.h
 * @brief Low-level hardware abstraction layer for the RD-03D radar.
 *
 * This file provides the direct hardware interfacing functions, handling
 * UART initialization, raw byte reading, and hardware-level error reporting.
 */
#pragma once
#include "rd_handle.h"
#include <stdint.h>

/**
 * @brief Status codes for the low-level hardware API.
 */
typedef enum {
	DEVICE_STATUS_OK = 0,
	DEVICE_STATUS_NOT_INITIALIZED,
	DEVICE_STATUS_FIFO_OVERFLOW,
	DEVICE_STATUS_RING_OVERFLOW,
	DEVICE_STATUS_READ_ERROR,
	DEVICE_STATUS_INVALID_PARAMETERS,
	DEVICE_STATUS_CMD_ERROR,
} rd_low_api_status_t;

/**
 * @brief Initializes the hardware UART interface for the radar.
 *
 * Configures the baud rate, pins, and installs the ESP-IDF UART driver
 * using the parameters specified in the device handle.
 *
 * @param handle_init Pointer to the radar device handle containing configuration parameters.
 * @return rd_low_api_status_t DEVICE_STATUS_OK on success, or an appropriate hardware error code.
 */
rd_low_api_status_t device_init(rd_handle_init_t *handle_init);

/**
 * @brief Reads raw data from the UART hardware queue.
 *
 * Blocks and waits for incoming UART events. When data arrives, it reads the raw bytes
 * into the handle's internal raw buffer and manages hardware errors like overflows.
 *
 * @param rd_handle Pointer to the initialized radar device handle.
 * @return rd_low_api_status_t DEVICE_STATUS_OK if bytes were successfully read, or an error code.
 */
rd_low_api_status_t device_read(rd_handle_init_t *rd_handle);

rd_low_api_status_t device_send_command(rd_handle_init_t *rd_handle, const uint8_t *command, size_t command_length, rd_handle_cmd_t type_command, size_t cmd_response_length);
