#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include "freertos/queue.h"
#include "types.h"

// Constants defining the range for random time generation
#define MIN_TIME_SECONDS 10
#define MAX_TIME_SECONDS 120


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
void generateRandomCarPlate(uint8_t *plate,size_t car_plate_len) {
    snprintf((char*)plate, car_plate_len, "%c%c%c%c%c%c%c",
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
    generateRandomCarPlate(car->plate, CAR_PLATE_LENGTH);

    // Generate a random time parked
    car->time_parked = generateRandomTime(MAX_TIME_SECONDS, MIN_TIME_SECONDS);
}

// Function to calculate the number of free parking slots
uint8_t countFreeParkingSlots(parking_spot_t* parking_lot, size_t capacity) {
    uint8_t freeSlots = 0;

    for (int i = 0; i < capacity; ++i) {
        if (parking_lot[i].status == EMPTY) {
            freeSlots++;
        }
    }

    return freeSlots;
}

// Function to find the index of the first empty parking slot
int8_t findFirstEmptyParkingSlotIndex(parking_spot_t* parking_lot, size_t capacity) {
    for (size_t i = 0; i < capacity; ++i) {
        if (parking_lot[i].status == EMPTY) {
            // Found an empty parking slot, return its index
            return i;
        }
    }
    // No empty parking slot found
    return -1;
}

uint32_t calculateParkingFee(uint16_t entrance_time, uint16_t exit_time, uint16_t time_fee){
    uint16_t time_spent = exit_time - entrance_time;
    uint16_t fee_value = time_spent* time_fee;
    return fee_value;
}

#endif  // _UTILS_H_