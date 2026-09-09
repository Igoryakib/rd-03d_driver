/**
 * @file rd-03d_low.c
 * @brief Implementation of the low-level hardware abstraction layer for the RD-03D radar.
 */
#include "rd-03d_low.h"

/* --- Configuration Macros --- */
#define RADAR_BAUD_RATE    256000
#define UART_BUFFER_SIZE   (1024 * 2)

/* --- Protocol Macros --- */
#define HEADER_FRAME 0xAAFF0300
#define TAIL_FRAME 0x55CC
#define SIZE_TARGETS_DATA 28

rd_low_api_status_t device_init(rd_handle_init_t *rd_handle) {
	rd_low_api_status_t statusCode = DEVICE_STATUS_OK;

	if (NULL == rd_handle) {
		statusCode = DEVICE_STATUS_INVALID_PARAMETERS;
	}

	if (DEVICE_STATUS_OK == statusCode) {
		uart_config_t uart_config = { .baud_rate = RADAR_BAUD_RATE, .data_bits =
				UART_DATA_8_BITS, .parity = UART_PARITY_DISABLE, .stop_bits =
				UART_STOP_BITS_1, .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
				.source_clk = UART_SCLK_DEFAULT, };

		if (ESP_OK != uart_param_config(rd_handle->uart_num, &uart_config)) {
			statusCode = DEVICE_STATUS_NOT_INITIALIZED;
		}

		if (DEVICE_STATUS_OK == statusCode
				&& ESP_OK
						!= uart_set_pin(rd_handle->uart_num,
								rd_handle->tx_io_num, rd_handle->rx_io_num,
								UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE)) {
			statusCode = DEVICE_STATUS_NOT_INITIALIZED;
		}

		if (DEVICE_STATUS_OK == statusCode
				&& ESP_OK != uart_driver_install(rd_handle->uart_num,
				UART_BUFFER_SIZE,
				UART_BUFFER_SIZE, 20, &rd_handle->uart_queue, 0)) {
			statusCode = DEVICE_STATUS_NOT_INITIALIZED;
		}

		if (DEVICE_STATUS_OK == statusCode
				&& ESP_OK != uart_flush_input(rd_handle->uart_num)) {
			statusCode = DEVICE_STATUS_NOT_INITIALIZED;
		}

	}

	return statusCode;
}

rd_low_api_status_t device_read(rd_handle_init_t *rd_handle) {
	rd_low_api_status_t statusCode = DEVICE_STATUS_OK;
	uart_event_t event;
	uint8_t raw_buffer[30] = {0};
	if (xQueueReceive(rd_handle->uart_queue, (void*) &event,
			(TickType_t) portMAX_DELAY)) {

		switch (event.type) {
		case UART_DATA:
			uart_read_bytes(rd_handle->uart_num, raw_buffer, event.size,
					portMAX_DELAY);
		uint16_t buf_tail_frame = (raw_buffer[28] << 8) | raw_buffer[29];
		uint32_t buf_header_frame = 0;
		uint8_t head_position = 24;
		for (int8_t i = 0; i < 4; head_position-=8, i++) {
			buf_header_frame |= raw_buffer[i] << head_position;
		}

		if (HEADER_FRAME == buf_header_frame && TAIL_FRAME == buf_tail_frame) {
			for (int8_t j= 0, i = 4; i < SIZE_TARGETS_DATA; j++, i++) {
				rd_handle->data_buffer[j] = raw_buffer[i];
			}
			statusCode = DEVICE_STATUS_OK;
		} else {
			statusCode = DEVICE_STATUS_READ_ERROR;
			uart_flush_input(rd_handle->uart_num);
			xQueueReset(rd_handle->uart_queue);
		}

			break;
		case UART_FIFO_OVF:
			statusCode = DEVICE_STATUS_FIFO_OVERFLOW;
			printf("Помилка: Hardware FIFO Overflow\n");
			uart_flush_input(rd_handle->uart_num);
			xQueueReset(rd_handle->uart_queue);
			break;

		case UART_BUFFER_FULL:
			statusCode = DEVICE_STATUS_RING_OVERFLOW;
			printf("Помилка: Ring Buffer Full\n");
			uart_flush_input(rd_handle->uart_num);
			xQueueReset(rd_handle->uart_queue);
			break;
		default:
			break;
		}
	}
	return statusCode;
}

