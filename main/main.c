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

//TODO:
//  - Add counter for cars that were in the parking lot
uint16_t total_cars = 0;
uint8_t parked_cars_quantity = 0;
uint32_t received_money = 0;

//  - Add counter for cars that are in the parking lot
//  - Add counter for money received in the parking lot
//  - Add gatemoneys to control the flow of cars

//Fazer esta função ocorrer quando o botão de boot da ESP32 for pressionado
esp_err_t new_car_enter(){
    ESP_LOGI("PARKING_LOT","Carro entrou no estacionamento!");
    return ESP_OK;
}

//Calcula o valor de
esp_err_t car_leaved(){
    ESP_LOGI("PARKING_LOT","Carro saiu do estacionamento!");
    return ESP_OK;
}

// Interrupt service routine for the GPIO pin
void IRAM_ATTR button_isr_handler(void* arg)
{
    // Get the GPIO pin number from the argument
    uint32_t gpio_num = (uint32_t)arg;
    
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void car_task(void *arg){
    //init car
    //Generates a random car plate
    //verifies if there are empty parking spots
    //if there are empty parking spots:
    //  Generates random number from range 5 seconds - 20 seconds
    //  Waits till that time passes
    //  Leaves and pays
    //if there's no empty parking spot
    //  Goes away
}

static void gpio_read_task(void* arg){
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            gpio_isr_handler_remove(CAR_ENTERED_BTN);
            xTaskCreate(car_task, "car_task", 2048, NULL, 10, NULL);
            vTaskDelay(pdMS_TO_TICKS(1000)); //Debouncing treatment.
            gpio_isr_handler_add(CAR_ENTERED_BTN, button_isr_handler, (void*) CAR_ENTERED_BTN);
        }
    }
}


void app_main(void)
{
    gpio_set_direction(CAR_ENTERED_BTN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(CAR_ENTERED_BTN, GPIO_PULLUP_ONLY);

    //change gpio interrupt type for one pin
    gpio_set_intr_type(CAR_ENTERED_BTN, GPIO_INTR_NEGEDGE);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(CAR_ENTERED_BTN, button_isr_handler, (void*) CAR_ENTERED_BTN);

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(gpio_read_task, "gpio_read_task", 2048, NULL, 2, NULL);
}