#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdio.h>
#define CAR_PLATE_LENGTH 9

enum car_status {
    WAITING_IN_QUEUE_TO_PARK,
    PARKED,
    WAITING_IN_QEUE_TO_LEAVE,
};

enum parking_slot_status {
    FREE_SLOT,
    OCCUPIED,
};

typedef struct car_struct {
    uint16_t time_parked;
    uint16_t entrance_time;
    uint8_t plate[CAR_PLATE_LENGTH];
    uint8_t status;
} car_t;

typedef struct parking_slot_struct {
    uint8_t status;
    car_t *parked_car;
} parking_slot_t;

#endif  // _TYPES_H_