/**
 * @file rd-03d_high.h
 * @brief High-level API for the RD-03D radar module.
 *
 * This file provides structures, enumerations, and function prototypes
 * for initializing the radar, reading data, and extracting target metrics 
 * (coordinates, velocity, and distance).
 */
#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include <string.h>
#include <stdbool.h>

/**
 * @brief Status codes for the high-level radar API.
 */
typedef enum {
	RD_STATUS_OK = 0,
	RD_STATUS_NOT_INITIALIZED,
	RD_STATUS_INITIALIZED,
	RD_STATUS_READ_ERROR,
	RD_STATUS_WRITE_ERROR,
	RD_STATUS_INVALID_PARAMETERS
} rd_high_api_status_t;

typedef enum {
	MODE_SINGLE_TARGTE,
	MODE_MULTI_TARGET
} rd_high_api_mode_t;

/**
 * @brief Status codes for target detection.
 */
typedef enum {
	RD_TARGET_CONFIRMED = 0,
	RD_NO_TARGET
} rd_high_target_status_t;

/**
 * @brief Maximum number of targets the RD-03D radar can track simultaneously.
 */
#define RD_MAX_TARGETS 3

/**
 * @struct rd_target_t
 * @brief Represents the physical metrics of a single detected target.
 */
typedef struct {
    int16_t x;          
    int16_t y;         
    int16_t velocity;
    uint16_t distance;     
    uint8_t id;         
} rd_target_t; 

/**
 * @struct rd_data_t
 * @brief Represents a complete frame of radar data containing multiple targets.
 */
typedef struct {
    rd_target_t targets[RD_MAX_TARGETS]; 
    uint8_t active_count;                
} rd_data_t;

/**
 * @typedef rd_handle_init_t
 * @brief Opaque pointer to the radar device handle.
 */
typedef struct rd_handle_init_s rd_handle_init_t;

/**
 * @brief Creates and allocates memory for a new radar device entity.
 * 
 * @param uart_num Hardware UART port number (e.g., UART_NUM_2).
 * @param tx_io_num GPIO pin number for UART TX.
 * @param rx_io_num GPIO pin number for UART RX.
 * @param uart_queue FreeRTOS queue for internal UART events.
 * @param data_queue FreeRTOS queue for passing parsed `rd_data_t` frames to the user application.
 * @return rd_handle_init_t* Pointer to the allocated device handle, or NULL if memory allocation failed.
 */
rd_handle_init_t *createEntity(uart_port_t uart_num, int tx_io_num,
							   int rx_io_num, QueueHandle_t uart_queue,
							   QueueHandle_t data_queue);

/**
 * @brief Initializes the hardware interfaces for the radar device.
 * 
 * @param handle_init Pointer to the radar device handle created by createEntity().
 * @return rd_high_api_status_t RD_STATUS_OK on success, or an error code on failure.
 */
rd_high_api_status_t rd_init(rd_handle_init_t *handle_init);

/**
 * @brief Reads incoming UART data, parses the frame, and dispatches it to the data queue.
 * 
 * @param rd_handle Pointer to the initialized radar device handle.
 * @return rd_high_api_status_t RD_STATUS_OK if a frame was successfully read and parsed, or an error code.
 */
rd_high_api_status_t rd_read(rd_handle_init_t *rd_handle);
