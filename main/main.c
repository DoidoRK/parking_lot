#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "config.h"

#define ESP_INTR_FLAG_DEFAULT 0

static QueueHandle_t gpio_evt_queue = NULL;

esp_err_t new_car_enter(){
    ESP_LOGI("PARKING_LOT","Carro entrou no estacionamento!");
    return ESP_OK;
}

esp_err_t car_leaved(){
    ESP_LOGI("PARKING_LOT","Carro saiu do estacionamento!");
    return ESP_OK;
}

esp_err_t print_total_receipt(){
    ESP_LOGI("PARKING_LOT","Recibo impresso!");
    return ESP_OK;
}

// Interrupt service routine for the GPIO pin
void IRAM_ATTR button_isr_handler(void* arg)
{
    // Get the GPIO pin number from the argument
    uint32_t gpio_num = (uint32_t)arg;
    
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_read_task(void* arg)
{
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            printf("GPIO[%"PRIu32"] intr, val: %d\n", gpio_num, gpio_get_level(gpio_num));
            //Debouncing treatment.
            gpio_isr_handler_remove(CAR_ENTERED_BTN);
            gpio_isr_handler_remove(CAR_LEAVED_BTN);
            gpio_isr_handler_remove(PRINT_RECEIPT_BTN);
            vTaskDelay(pdMS_TO_TICKS(300));
            gpio_isr_handler_add(CAR_ENTERED_BTN, button_isr_handler, (void*) CAR_ENTERED_BTN);
            gpio_isr_handler_add(CAR_LEAVED_BTN, button_isr_handler, (void*) CAR_LEAVED_BTN);
            gpio_isr_handler_add(PRINT_RECEIPT_BTN, button_isr_handler, (void*) PRINT_RECEIPT_BTN);
        }
    }
}


void app_main(void)
{
    gpio_set_direction(CAR_ENTERED_BTN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(CAR_ENTERED_BTN, GPIO_PULLUP_ENABLE);
    gpio_set_direction(CAR_LEAVED_BTN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(CAR_LEAVED_BTN, GPIO_PULLUP_ENABLE);
    gpio_set_direction(PRINT_RECEIPT_BTN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PRINT_RECEIPT_BTN, GPIO_PULLUP_ENABLE);

    //change gpio interrupt type for one pin
    gpio_set_intr_type(CAR_ENTERED_BTN, GPIO_INTR_LOW_LEVEL);
    gpio_set_intr_type(CAR_LEAVED_BTN, GPIO_INTR_LOW_LEVEL);
    gpio_set_intr_type(PRINT_RECEIPT_BTN, GPIO_INTR_LOW_LEVEL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(gpio_read_task, "gpio_read_task", 2048, NULL, 10, NULL);
    
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(CAR_ENTERED_BTN, button_isr_handler, (void*) CAR_ENTERED_BTN);
    gpio_isr_handler_add(CAR_LEAVED_BTN, button_isr_handler, (void*) CAR_LEAVED_BTN);
    gpio_isr_handler_add(PRINT_RECEIPT_BTN, button_isr_handler, (void*) PRINT_RECEIPT_BTN);

}