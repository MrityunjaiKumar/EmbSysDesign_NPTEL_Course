/*
 * LunchBox_utility.c
 *
 *  Created on: Feb 26, 2020
 *      Author: sincgrid
 */

#include <Library/LunchboxCommon.h>
#include <msp430.h>

#include <stdio.h>
#include <string.h>

// Function that convert Decimal to binary
void decToBinary(int n)
{
    int i;
    for (i = 8; i >= 0; i--) {
        int k = n >> i;
        if (k & 1)
            printf("1");
        else
            printf("0");
    }
}
