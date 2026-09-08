#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "driver/gpio.h"

const typedef struct {
	uart_port_t uart_num;
	int tx_io_num;
	int rx_io_num;
	QueueHandle_t *uart_queue;
} rd_hadnle_init_t;
