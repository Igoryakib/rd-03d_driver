#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "driver/gpio.h"

typedef enum {
	RD_STATUS_OK = 0,
	RD_STATUS_NOT_INITIALIZED,
	RD_STATUS_INITIALIZED,
	RD_STATUS_READ_ERROR,
	RD_STATUS_WRITE_ERROR,
	RD_STATUS_INVALID_PARAMETERS
} rd_high_api_status_t;

typedef struct rd_handle_init_s rd_handle_init_t;

rd_handle_init_t *createEntity(uart_port_t uart_num, int tx_io_num,
							   int rx_io_num, QueueHandle_t uart_queue,
							   QueueHandle_t data_queue);

rd_high_api_status_t rd_init(rd_handle_init_t *handle_init);

rd_high_api_status_t rd_read(rd_handle_init_t *rd_handle);
