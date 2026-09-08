#include "rd_handle.h"
#include "rd-03d_high.h"
#include "rd-03d_low.h"

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

	if(RD_STATUS_OK == statusCode && RD_STATUS_INITIALIZED != rd_handle->initialize_status) {
		statusCode = RD_STATUS_NOT_INITIALIZED;
	}

	if (RD_STATUS_OK == statusCode) {
		if (DEVICE_STATUS_OK == device_read(rd_handle)) {
			for (int8_t i = 0; i < 24; i++) {
				printf("%02X ", rd_handle->data_buffer[i]);
			}
			printf("\n");
		} else {
			statusCode = RD_STATUS_READ_ERROR;
		}
	}

	return statusCode;
}
