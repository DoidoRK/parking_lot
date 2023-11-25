#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>
#include <stdlib.h>
#include "freertos/queue.h"
#include "types.h"

// Constants defining the range for random time generation
#define MIN_TIME_SECONDS 5
#define MAX_TIME_SECONDS 30

// Function to check if the queue is full
uint8_t isQueueFull(QueueHandle_t queue) {
    UBaseType_t spacesAvailable = uxQueueSpacesAvailable(queue);
    return spacesAvailable == 0;
}

// Function to convert car status to a corresponding message
const char* carStatusToMessage(uint8_t status) {
    switch (status) {
        case WAITING_IN_QUEUE_TO_PARK:
            return "Waiting in queue to park";
        case PARKED:
            return "Parked";
        case WAITING_IN_QEUE_TO_LEAVE:
            return "Waiting in queue to leave";
        default:
            return "Unknown status";
    }
}

// Function to generate a random time in milliseconds within a specified range
int generateRandomTime() {
    // Generate a random number of seconds within the defined range
    int randomSeconds = rand() % (MAX_TIME_SECONDS - MIN_TIME_SECONDS + 1) + MIN_TIME_SECONDS;

    // Convert seconds to milliseconds
    int randomMilliseconds = randomSeconds * 1000;

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

// Function to calculate the number of free parking slots
uint8_t countFreeParkingSlots(parking_slot_t parking_lot[PARKING_SPOT_QUANTITY]) {
    uint8_t freeSlots = 0;

    for (int i = 0; i < PARKING_SPOT_QUANTITY; ++i) {
        if (parking_lot[i].status == FREE_SLOT) {
            freeSlots++;
        }
    }

    return freeSlots;
}

#endif  // _UTILS_H_