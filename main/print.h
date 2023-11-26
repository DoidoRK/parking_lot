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
    uint8_t *car_in_exit_plate,
    uint32_t received_money,
    uint8_t enter_queue_size,
    uint8_t exit_queue_size
    ){
        gotoxy(x,y);
        printf("HORÁRIO ATUAL:%"PRIu32"", received_money);
        gotoxy(x,y+1);
        printf("TOTAL ARRECADADO: R$%"PRIu32"", received_money);
        gotoxy(x,y+2);
        printf("CARROS ESTACIONADOS: %d/%d", parked_cars_quantity, parking_lot_max_capacity);
        gotoxy(x,y+3);
        printf("TOTAL DE CARROS ATENDIDOS: %d", total_cars);
        gotoxy(x,y+4);
        printf("CARROS NA FILA DE ENTRADA: %d", enter_queue_size);
        gotoxy(x,y+5);
        printf("CARROS NA FILA DE SAÍDA: %d", exit_queue_size);
}

void printGate( uint8_t x, uint8_t y, char* gate_name, uint8_t *car_plate, uint8_t is_empty){
    if(is_empty){
        setfontcolor(RED);
    } else {
        setfontcolor(GREEN);
    }
    gotoxy(x,y);
    printf("╔════════════════════╗");
    gotoxy(x,y+1);
    printf("║ %s ║",gate_name);
    gotoxy(x,y+2);
    if (is_empty)
    {
        printf("║       -------      ║");
    } else {
        printf("║       %s      ║",car_plate);
    }
    gotoxy(x,y+3);
    printf("╚════════════════════╝");
    setfontcolor(WHITE);
}

void printParkingSpot(uint8_t x, uint8_t y, uint8_t *car_plate, uint8_t is_empty){
    if(is_empty){
        setfontcolor(GREEN);
    } else {
        setfontcolor(RED);
    }
    gotoxy(x,y);
    printf("┌───────┐");
    gotoxy(x,y+1);
    if(is_empty){
        printf("│-------│");
    } else {
        printf("│%s│",car_plate);
    }
    gotoxy(x,y+2);
    printf("└───────┘");
    setfontcolor(WHITE);
}


void printParkingLot(uint8_t x, uint8_t y){
    gotoxy(x,y);
    printf("╔═══════════════════════════════════════════════╗");
    gotoxy(x,y+1);
    printf("║           VAGAS DE ESTACIONAMENTO             ║");
    gotoxy(x,y+2);
    printf("║                                               ║");
    gotoxy(x,y+3);
    printf("║                                               ║");
    gotoxy(x,y+4);
    printf("║                                               ║");
    gotoxy(x,y+5);
    printf("║                                               ║");
    gotoxy(x,y+6);
    printf("║                                               ║");
    gotoxy(x,y+7);
    printf("║                                               ║");
    gotoxy(x,y+8);
    printf("║                                               ║");
    gotoxy(x,y+9);
    printf("║                                               ║");
    gotoxy(x,y+10);
    printf("║                                               ║");
    gotoxy(x,y+11);
    printf("║                                               ║");
    gotoxy(x,y+12);
    printf("║                                               ║");
    gotoxy(x,y+13);
    printf("║                                               ║");
    gotoxy(x,y+14);
    printf("╚═══════════════════════════════════════════════╝");
}

#endif  // _PRINT_H_