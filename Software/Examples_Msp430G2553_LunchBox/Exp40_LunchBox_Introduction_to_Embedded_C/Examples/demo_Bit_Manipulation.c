/*
 * demo_Bit_Manipulation.c
 *
 *  Created on: Feb 26, 2020
 *      Author: sincgrid
 */

#include "Example_common.h"
#include <inttypes.h>
#include <stdbool.h>

void demo_bit_manipulation()
{

    uint8_t x = 2;
    uint8_t y = 4;
    uint8_t z = 8;
    printf("\r\n x value \t");
    decToBinary(x);
    printf("\r\n y value \t");
    decToBinary(y);
    printf("\r\n z value \t");
    decToBinary(z);
    printf("\r\n\r\n\r\n");


    printf("\r\n Bit opeartors in C \r\n");
    uint8_t a=0b01000100; //68 or 0x44
    uint8_t b=0b00100100; //36 or 0x24
    printf("\r\n value of a & b \t");decToBinary(a&b);
    printf("\r\n value of a | b \t");decToBinary(a|b);
    printf("\r\n value of a<<1 \t");decToBinary(a<<1);
    printf("\r\n value of a^b \t");decToBinary(a^b);
    printf("\r\n\r\n\r\n");


#define _BV(bit) (1 << (bit))
}

