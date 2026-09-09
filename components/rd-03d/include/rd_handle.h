/**
 * @file rd_handle.h
 * @brief Declaration of the device handle structure for the RD-03D radar.
 *
 * This file contains the definition of the radar driver's internal state,
 * including UART configurations, FreeRTOS queues, and frame parsing buffers.
 */
#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "rd-03d_high.h"
#include <stdint.h>

/**
 * @struct rd_handle_init_s
 * @brief Device descriptor for the RD-03D radar.
 * 
 * Stores the complete context required for hardware communication with the radar,
 * memory management, and thread-safe synchronization via FreeRTOS.
 */
struct rd_handle_init_s{
	QueueHandle_t uart_queue;  /**< Internal UART event queue managed by the ESP-IDF driver */
	QueueHandle_t data_queue; /**< FreeRTOS queue for passing parsed data frames to the user application */
	uart_port_t uart_num;    /**< Hardware UART port number (e.g., UART_NUM_2) */
	uint8_t data_buffer[24]; /**< Buffer for temporary storage of the frame payload (3 targets, 8 bytes each) */
	uint8_t size_buffer;  /**< Size of the valid data currently stored in the buffer */
	gpio_num_t tx_io_num; /**< GPIO pin number for TX (transmitting data to the radar) */
	gpio_num_t rx_io_num; /**< GPIO pin number for RX (receiving data from the radar) */
	rd_high_api_status_t initialize_status; /**< Current initialization status of the radar device */
};
