#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "config.h"
#include "types.h"
#include "utils.h"

#define ESP_INTR_FLAG_DEFAULT 0

static const char *MAIN_TAG = "PARKING_SYSTEM";

time_t globalSystemTime;    //Global time.

static QueueHandle_t gpio_evt_queue = NULL;
static QueueHandle_t entrance_car_queue = NULL;
static QueueHandle_t exit_car_queue = NULL;

uint16_t total_cars = 0;
uint8_t parked_cars_quantity = 0;
uint32_t received_money = 0;
uint8_t entrance_gate = 0;
uint8_t exit_gate = 0;

SemaphoreHandle_t parking_lot_semaphore;

parking_slot_t parking_lot[PARKING_SPOT_QUANTITY];

// Function to initiate a car in the WAITING_IN_QUEUE_TO_PARK state
void initiateCar(car_t *car) {
    // Set the car status to WAITING_IN_QUEUE_TO_PARK
    car->status = WAITING_IN_QUEUE_TO_PARK;

    // Generate a random car plate
    generateRandomCarPlate(car->plate);

    // Generate a random time parked
    car->time_parked = generateRandomTime();
}

// Interrupt service routine for the GPIO pin
void IRAM_ATTR button_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void car_task(void *arg){
    while(true){
    }
}

static void gpio_read_task(void* arg){
    uint32_t io_num;
    while(true) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            gpio_isr_handler_remove(CAR_ENTERED_BTN);
            car_t new_car;
            initiateCar(&new_car);
            xQueueSend(entrance_car_queue, &new_car, portMAX_DELAY);
            vTaskDelay(pdMS_TO_TICKS(DEBOUNCING_TIME)); //Button press debouncing treatment.
            gpio_isr_handler_add(CAR_ENTERED_BTN, button_isr_handler, (void*) CAR_ENTERED_BTN);
        }
    }
}

static void entrance_gate_task(void* arg){
    car_t entering_car;
    while(true) {
        if(xQueueReceive(entrance_car_queue, &entering_car, portMAX_DELAY)) {
            xSemaphoreTake(parking_lot_semaphore, portMAX_DELAY);
            xSemaphoreGive(parking_lot_semaphore);
            // gpio_isr_handler_remove(CAR_ENTERED_BTN);
            // xTaskCreate(car_task, "car_task", 2048, NULL, 10, NULL);
            // vTaskDelay(pdMS_TO_TICKS(1000)); //Debouncing treatment.
            // gpio_isr_handler_add(CAR_ENTERED_BTN, button_isr_handler, (void*) CAR_ENTERED_BTN);
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


    parking_lot_semaphore = xSemaphoreCreateMutex();
    if (parking_lot_semaphore == NULL) {
        ESP_LOGE(MAIN_TAG, "Failed to create parking lot semaphore");
        exit(1);
    }
    xSemaphoreGive(parking_lot_semaphore);

    // ESP_ERROR_CHECK(init_trains());
    // ESP_ERROR_CHECK(init_stations());
    // clrscr();

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    entrance_car_queue = xQueueCreate(10, sizeof(uint32_t));
    exit_car_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(gpio_read_task, "gpio_read_task", 2048, NULL, 2, NULL);
}