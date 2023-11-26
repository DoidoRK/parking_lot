#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdio.h>
#define CAR_PLATE_LENGTH 8

enum parking_slot_status {
    EMPTY,
    OCCUPIED,
};

typedef struct car_struct {
    uint16_t time_parked;
    uint16_t entrance_time;
    int8_t  parking_spot;
    uint8_t plate[CAR_PLATE_LENGTH];
} car_t;

typedef struct parking_slot_struct {
    uint8_t status;
    car_t parked_car;
} parking_spot_t;

typedef struct receipt_struct {
    uint16_t entrance_time;
    uint16_t exit_time;
    uint32_t value;
    uint16_t total_time;
    uint8_t car_plate[CAR_PLATE_LENGTH];
} receipt_t;

#endif  // _TYPES_H_