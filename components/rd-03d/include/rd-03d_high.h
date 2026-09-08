#pragma once
#include "rd_handle.h"
#include "rd-03d_low.h"

typedef enum {
	RD_STATUS_OK = 0,
	RD_STATUS_NOT_INITIALIZED,
	RD_STATUS_READ_ERROR,
	RD_STATUS_WRITE_ERROR,
	RD_STATUS_INVALID_PARAMETERS
} rd_high_api_status_t;


rd_high_api_status_t rd_init(rd_hadnle_init_t *const handle_init);

rd_high_api_status_t rd_read();
