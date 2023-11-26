#ifndef _PRINT_H_
#define _PRINT_H_

#include <stdio.h>
#include <stdlib.h>
#include "conio_linux.h"
#include "types.h"

//Print settings
#define PRINT_DELAY 300

void printSystemTable(
    uint8_t x, uint8_t y,
    uint16_t total_cars,
    uint8_t parked_cars_quantity,
    uint8_t parking_lot_max_capacity,
    uint8_t *car_in_entrance_plate,
    // uint8_t *car_in_exit_plate,
    uint32_t received_money,
    uint8_t enter_queue_size,
    uint8_t exit_queue_size
    ){
        gotoxy(x,y);
        printf("HORÁRIO ATUAL:[%"PRIu32"]", received_money);
        gotoxy(x,y+1);
        printf("TOTAL ARRECADADO: R$[%"PRIu32"]", received_money);
        gotoxy(x,y+2);
        printf("CARROS ESTACIONADOS: %d/%d", parked_cars_quantity, parking_lot_max_capacity);
        gotoxy(x,y+3);
        printf("TOTAL DE CARROS ATENDIDOS: %d", total_cars);
        gotoxy(x,y+4);
        printf("CARRO NA CANCELA DE ENTRADA: %s", car_in_entrance_plate);
        gotoxy(x,y+5);
        // printf("CARRO NA CANCELA DE SAÍDA: %s", car_in_exit_plate);
        gotoxy(x,y+6);
        printf("CARROS NA FILA DE ENTRADA: %d", enter_queue_size);
        gotoxy(x,y+7);
        printf("CARROS NA FILA DE SAÍDA: %d", exit_queue_size);
}

// void printEntranceQueue(
//     uint8_t x, uint8_t y,
//     car_t entrance_queue[],
//     car_t entrance_return_queue[],
//     size_t queue_size,
// ){

// }

// void printParkingLot(){

// }


// void printExitQueue(){

// }

#endif  // _PRINT_H_