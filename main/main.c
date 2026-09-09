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
	data_queue = xQueueCreate(10, sizeof(rd_data_t));
	rd_handle_init_t *rd_handle_init = createEntity(UART_PORT_NUM, RADAR_TXD_PIN, RADAR_RXD_PIN, uart_queue, data_queue);
	if (RD_STATUS_OK == rd_init(rd_handle_init)) {
		printf("UART ініціалізовано. Швидкість: 256000 bps. Очікування даних від RD-03D...\n");
	}
	rd_data_t radar_data;
	
    while (true) {
		if (RD_STATUS_OK == rd_read(rd_handle_init) && xQueueReceive(data_queue, &radar_data, portMAX_DELAY) == pdTRUE) {
			printf("--- Новий кадр! Активних цілей: %d ---\n", radar_data.active_count);

            for (int i = 0; i < 3; i++) {
                if (radar_data.targets[i].id != 0) {
                    printf("Ціль %d -> X: %d мм | Y: %d мм | Швидкість: %d см/с | Відстань: %d мм\n",
                           radar_data.targets[i].id,
                           radar_data.targets[i].x,
                           radar_data.targets[i].y,
                           radar_data.targets[i].velocity,
                           radar_data.targets[i].distance);
                }
            }
		}
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
