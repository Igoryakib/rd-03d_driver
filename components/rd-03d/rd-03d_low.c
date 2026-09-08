#include "rd-03d_low.h"

#define RADAR_BAUD_RATE    256000
#define UART_BUFFER_SIZE   (1024 * 2)

rd_low_api_status_t device_init(rd_hadnle_init_t *const handle_init) {
	rd_low_api_status_t statusCode = DEVICE_STATUS_OK;

	if (NULL == handle_init) {
		statusCode = DEVICE_STATUS_INVALID_PARAMETERS;
	}

	if (DEVICE_STATUS_OK == statusCode) {
		uart_config_t uart_config = { .baud_rate = RADAR_BAUD_RATE, .data_bits =
				UART_DATA_8_BITS, .parity = UART_PARITY_DISABLE, .stop_bits =
				UART_STOP_BITS_1, .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
				.source_clk = UART_SCLK_DEFAULT, };

		if (ESP_OK != uart_param_config(handle_init->uart_num, &uart_config)) {
			statusCode = DEVICE_STATUS_NOT_INITIALIZED;
		}

		if (DEVICE_STATUS_OK == statusCode && ESP_OK
				!= uart_set_pin(handle_init->uart_num, handle_init->tx_io_num,
						handle_init->rx_io_num, UART_PIN_NO_CHANGE,
						UART_PIN_NO_CHANGE)) {
			statusCode = DEVICE_STATUS_NOT_INITIALIZED;
		}

		if (DEVICE_STATUS_OK == statusCode && ESP_OK
				!= uart_driver_install(handle_init->uart_num, UART_BUFFER_SIZE,
						UART_BUFFER_SIZE, 20, handle_init->uart_queue, 0)) {
			statusCode = DEVICE_STATUS_NOT_INITIALIZED;
		}

	}

	return statusCode;
}

