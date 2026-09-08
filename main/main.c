#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "rd-03d_high.h"

#define RADAR_TXD_PIN (GPIO_NUM_17)
#define RADAR_RXD_PIN (GPIO_NUM_16)
#define UART_PORT_NUM      UART_NUM_2


static QueueHandle_t uart_queue;
static QueueHandle_t data_queue;

void app_main(void)
{
	rd_handle_init_t *rd_handle_init = createEntity(UART_PORT_NUM, RADAR_TXD_PIN, RADAR_RXD_PIN, uart_queue, data_queue);
	if (RD_STATUS_OK == rd_init(rd_handle_init)) {
		printf("UART ініціалізовано. Швидкість: 256000 bps. Очікування даних від RD-03D...\n");
	}
    while (true) {
		rd_read(rd_handle_init);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
