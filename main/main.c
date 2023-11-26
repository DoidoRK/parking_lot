#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "time.h"
#include "esp_log.h"
#include "config.h"
#include "types.h"
#include "utils.h"
#include "print.h"

#define ESP_INTR_FLAG_DEFAULT 0

static const char *MAIN_TAG = "PARKING_SYSTEM";

time_t globalSystemTime;    //Global time.

static QueueHandle_t gpio_evt_queue = NULL;
static QueueHandle_t entrance_queue = NULL;
static QueueHandle_t exit_queue = NULL;

SemaphoreHandle_t parking_lot_semaphore;

uint16_t total_cars = 0;
uint8_t parked_cars_quantity = 0;
uint32_t received_money = 0;
car_t car_in_entrance_gate, car_in_exit_gate;
const parking_slot_t empty_parking_slot = {
    .status = EMPTY_PARKING_SLOT,
    .parked_car = {
        .time_parked = 0,
        .plate = {32,32,32,32,32,32,32,32,32},
    },
};
const car_t empty_car = {
    .entrance_time = 0,
    .time_parked = 0,
    .plate = {32,32,32,32,32,32,32,32,32},
};

parking_slot_t parking_lot[PARKING_LOT_CAPACITY];

// Interrupt service routine for the GPIO pin
void IRAM_ATTR buttonISRHandler(void* arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    if(gpio_evt_queue)
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void carTask(void *arg){
    car_t *p_car = (car_t *)arg;
    vTaskDelay(pdMS_TO_TICKS(p_car->time_parked));
    printf("Tempo do carro passou!\n");
    printf("Car Plate: %s\n", p_car->plate);
    printf("Time Parked: %d milliseconds\n", p_car->time_parked);
    // xSemaphoreTake(parking_lot_semaphore, portMAX_DELAY);
    //Searches for car in the parking slot and then 
    // xSemaphoreGive(parking_lot_semaphore);
    // free(p_car);
}

static void gpioReadTask(void* arg){
    uint32_t io_num;
    while(true) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            gpio_isr_handler_remove(CAR_ENTERED_BTN);
            car_t new_car;
            initiateCar(&new_car);
            if(!isQueueFull(entrance_queue)){
                xQueueSend(entrance_queue, &new_car, pdMS_TO_TICKS(CAR_SPEED));
            } else {
                printf("Fila de entrada cheia! nenhum outro carro pode entrar!\n");
            }
            vTaskDelay(pdMS_TO_TICKS(DEBOUNCING_TIME)); //Button press debouncing treatment.
            gpio_isr_handler_add(CAR_ENTERED_BTN, buttonISRHandler, (void*) CAR_ENTERED_BTN);
        }
    }
}

static void entranceGateTask(void* arg){
    while(true) {
            if(xQueueReceive(entrance_queue, &car_in_entrance_gate, portMAX_DELAY)) {
                //Checks if there are free spaces in the parking slot
                // printf("Carro na cancela de entrada!\n");
                // printf("Car Plate: %s\n", car_in_entrance_gate.plate);
                // printf("Time Parked: %d milliseconds\n", car_in_entrance_gate.time_parked);
                xSemaphoreTake(parking_lot_semaphore, portMAX_DELAY);
                uint8_t free_parking_slots_quantity = countFreeParkingSlots(parking_lot);
                xSemaphoreGive(parking_lot_semaphore);
                if(0 < free_parking_slots_quantity){
                    // printf("Carro entrou no estacionamento!\n");
                    car_in_entrance_gate.entrance_time = time(NULL);
                    vTaskDelay(pdMS_TO_TICKS(GATE_OPEN_TIME));
                    // xSemaphoreTake(parking_lot_semaphore, portMAX_DELAY);
                    // parking_lot[PARKING_LOT_CAPACITY - free_parking_slots_quantity].status = OCCUPIED;
                    // memcpy(&parking_lot[PARKING_LOT_CAPACITY - free_parking_slots_quantity].parked_car, &car_in_entrance_gate, sizeof(car_t));
                    // parked_cars_quantity++;
                    // xTaskCreate();
                    // xSemaphoreGive(parking_lot_semaphore);
                    // xSemaphoreTake(entrance_queue_semaphore, portMAX_DELAY);
                    // car_in_entrance_gate = empty_parking_slot;
                    // xSemaphoreGive(entrance_queue_semaphore);

                    vTaskDelay(pdMS_TO_TICKS(GATE_CLOSE_TIME));
                } else {
                    // printf("Carro rejeitado, sem espaço no estacionamento!\n");
                }
                memcpy(&car_in_entrance_gate, &empty_car, sizeof(car_t));
            }
    }
}

static void systemPrintTask(void* arg){
    uint8_t cars_in_entrance_queue = 0;
    while (true)
    {
        cars_in_entrance_queue = QUEUES_MAX_SIZE - uxQueueSpacesAvailable(entrance_queue);
        printSystemTable(
            0,0,
            total_cars,
            parked_cars_quantity,
            PARKING_LOT_CAPACITY,
            car_in_entrance_gate.plate,
            car_in_exit_gate.plate,
            received_money,
            cars_in_entrance_queue,
            QUEUES_MAX_SIZE
        );
        gotoxy(0,15);
        vTaskDelay(pdMS_TO_TICKS(PRINT_DELAY)); //Button press debouncing treatment.
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
    gpio_isr_handler_add(CAR_ENTERED_BTN, buttonISRHandler, (void*) CAR_ENTERED_BTN);


    parking_lot_semaphore = xSemaphoreCreateMutex();
    if (parking_lot_semaphore == NULL) {
        ESP_LOGE(MAIN_TAG, "Failed to create parking lot semaphore");
        exit(1);
    }
    xSemaphoreGive(parking_lot_semaphore);

    memcpy(&car_in_entrance_gate, &empty_car, sizeof(car_t));
    memcpy(&car_in_exit_gate, &empty_car, sizeof(car_t));
    clrscr();

    gpio_evt_queue = xQueueCreate(QUEUES_MAX_SIZE, sizeof(uint32_t));
    entrance_queue = xQueueCreate(QUEUES_MAX_SIZE, sizeof(car_t));
    exit_queue = xQueueCreate(QUEUES_MAX_SIZE, sizeof(car_t));

    xTaskCreate(gpioReadTask, "gpioReadTask", 2048, NULL, 2, NULL);
    xTaskCreate(entranceGateTask, "entranceGateTask", 2048, NULL, 3, NULL);
    xTaskCreate(systemPrintTask, "systemPrintTask", 2048, NULL, 10, NULL);
}