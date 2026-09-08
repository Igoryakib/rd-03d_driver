#pragma once
#include "rd_handle.h"
#include <stdint.h>

typedef enum {
	DEVICE_STATUS_OK = 0,
	DEVICE_STATUS_NOT_INITIALIZED,
	DEVICE_STATUS_FIFO_OVERFLOW,
	DEVICE_STATUS_RING_OVERFLOW,
	DEVICE_STATUS_READ_ERROR,
	DEVICE_STATUS_INVALID_PARAMETERS
} rd_low_api_status_t;

rd_low_api_status_t device_init(rd_handle_init_t *handle_init);

rd_low_api_status_t device_read(rd_handle_init_t *rd_handle);
