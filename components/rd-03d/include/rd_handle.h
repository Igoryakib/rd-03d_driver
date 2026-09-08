#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "rd-03d_high.h"
#include <stdint.h>
#include <stdbool.h>

struct rd_handle_init_s{
	QueueHandle_t uart_queue;
	QueueHandle_t data_queue;
	uart_port_t uart_num;
	uint8_t data_buffer[24];
	int8_t tx_io_num;
	int8_t rx_io_num;
	rd_high_api_status_t initialize_status;
};
