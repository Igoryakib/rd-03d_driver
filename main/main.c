#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "rd-03d_high.h"

#define RADAR_TXD_PIN (GPIO_NUM_17)
#define RADAR_RXD_PIN (GPIO_NUM_16)
#define UART_PORT_NUM      UART_NUM_2


static QueueHandle_t uart_queue;

void app_main(void)
{
	const rd_hadnle_init_t rd_handle_init = {
		.uart_num = UART_PORT_NUM,
		.tx_io_num = RADAR_TXD_PIN,
		.rx_io_num = RADAR_RXD_PIN,
		.uart_queue = &uart_queue 
	};
	if (RD_STATUS_OK == rd_init(&rd_handle_init)) {
		printf("UART ініціалізовано. Швидкість: 256000 bps. Очікування даних від RD-03D...\n");
	}
    uint8_t data[120];
    while (true) {
        int length = uart_read_bytes(UART_PORT_NUM, data, sizeof(data), pdMS_TO_TICKS(20));

        if (length > 0) {
            printf("Отримано %d байт: ", length);
            for (int i = 0; i < length; i++) {
                printf("%02X ", data[i]);
            }
            printf("\n");
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
