/*
 * demo_data_types.c
 *
 *  Created on: Feb 26, 2020
 *      Author: sincgrid
 */

#include "Example_common.h"
#include <inttypes.h>
#include <stdbool.h>
void demo_data_types()
{
    char a;
    signed char b;
    unsigned char c;

    bool d;
    int e;
    long f;
    uint8_t g;
    uint16_t h;
    uint32_t i;

    /*The sizeof operator returns the number of bytes
     *  occupied by a variable of a given type.*/
    printf("sizeof(a) =\t %d \r\n", sizeof(a));
    printf("sizeof(b) =\t %d \r\n", sizeof(b));
    printf("sizeof(c) =\t %d \r\n", sizeof(c));
    printf("sizeof(d) =\t %d \r\n", sizeof(d));
    printf("sizeof(e) =\t %d \r\n", sizeof(e));
    printf("sizeof(f) =\t %d \r\n", sizeof(f));
    printf("sizeof(g) =\t %d \r\n", sizeof(g));
    printf("sizeof(h) =\t %d \r\n", sizeof(h));
    printf("sizeof(i) =\t %d \r\n", sizeof(i));
    printf("\r\n\r\n\r\n\r\n");

}
