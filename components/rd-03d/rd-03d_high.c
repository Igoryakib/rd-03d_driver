#include "rd-03d_high.h"

rd_high_api_status_t rd_init(rd_hadnle_init_t *const handle_init) {
	rd_high_api_status_t statusCode = RD_STATUS_OK;

	if (NULL == handle_init) {
		statusCode = RD_STATUS_INVALID_PARAMETERS;
	}

	if (RD_STATUS_OK == statusCode) {
		if (DEVICE_STATUS_OK != device_init(handle_init)) {
			statusCode = RD_STATUS_NOT_INITIALIZED;
		}
	}

	return statusCode;
}
