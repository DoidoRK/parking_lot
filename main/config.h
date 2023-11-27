#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdio.h>
#include "driver/gpio.h"

//Parking lot settings
#define PARKING_LOT_CAPACITY 20
#define TIME_FEE 2
#define GATE_OPEN_TIME  4000
#define GATE_CLOSE_TIME 2000
#define CAR_SPEED 1000
#define QUEUES_MAX_SIZE 5

//GPIO settings
#define CAR_ENTERED_BTN GPIO_NUM_0
#define DEBOUNCING_TIME 300

#endif  // _CONFIG_H_