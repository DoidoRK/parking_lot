#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdio.h>
#include "driver/gpio.h"

//Parking lot settings
#define PARKING_SPOT_QUANTITY 20
#define VALUE_PER_TIME 20
#define GATE_OPEN_TIME  4000
#define GATE_CLOSE_TIME 2000
#define CAR_SPEED 500

//GPIO settings
#define CAR_ENTERED_BTN GPIO_NUM_0
#define DEBOUNCING_TIME 300

#endif  // _CONFIG_H_