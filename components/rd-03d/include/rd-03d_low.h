#pragma once
#include "rd_handle.h"

typedef enum {
	DEVICE_STATUS_OK = 0,
	DEVICE_STATUS_NOT_INITIALIZED,
	DEVICE_STATUS_FIFO_OVERFLOW,
	DEVICE_STATUS_RING_OVERFLOW,
	DEVICE_STATUS_INVALID_PARAMETERS
} rd_low_api_status_t;

rd_low_api_status_t device_init(rd_hadnle_init_t *const handle_init);
