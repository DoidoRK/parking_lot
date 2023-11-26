#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include "freertos/queue.h"
#include "types.h"

// Constants defining the range for random time generation
#define MIN_TIME_SECONDS 5
#define MAX_TIME_SECONDS 30


// Function to generate a random time in milliseconds within a specified range
uint16_t generateRandomTime(uint8_t max_time_seconds,uint8_t min_time_seconds) {
    // Generate a random number of seconds within the defined range
    uint16_t randomSeconds = rand() % (max_time_seconds - min_time_seconds + 1) + min_time_seconds;

    // Convert seconds to milliseconds
    uint16_t randomMilliseconds = randomSeconds * 1000;

    return randomMilliseconds;
}

// Function to generate a random uppercase letter
uint8_t randomLetter() {
    return (uint8_t)('A' + rand() % 26);
}

// Function to generate a random digit
uint8_t randomDigit() {
    return (uint8_t)('0' + rand() % 10);
}

// Function to generate a random Brazilian-style car plate and return it as a character array
void generateRandomCarPlate(uint8_t plate[CAR_PLATE_LENGTH]) {
    snprintf((char*)plate, CAR_PLATE_LENGTH, "%c%c%c%c%c%c%c",
             randomLetter(), randomLetter(), randomLetter(),
             randomDigit(),
             randomLetter(),
             randomDigit(), randomDigit());
}

uint8_t isQueueFull(QueueHandle_t queue) {
    UBaseType_t spacesAvailable = uxQueueSpacesAvailable(queue);
    return spacesAvailable == 0;
}

// Function to initiate a car
void initiateCar(car_t *car) {
    car->entrance_time = 0;

    // Generate a random car plate
    generateRandomCarPlate(car->plate);

    // Generate a random time parked
    car->time_parked = generateRandomTime(MAX_TIME_SECONDS, MIN_TIME_SECONDS);
}

// Function to calculate the number of free parking slots
uint8_t countFreeParkingSlots(parking_slot_t parking_lot[PARKING_LOT_CAPACITY]) {
    uint8_t freeSlots = 0;

    for (int i = 0; i < PARKING_LOT_CAPACITY; ++i) {
        if (parking_lot[i].status == EMPTY_PARKING_SLOT) {
            freeSlots++;
        }
    }

    return freeSlots;
}

#endif  // _UTILS_H_