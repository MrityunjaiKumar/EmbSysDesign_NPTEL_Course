#include <msp430.h>
#include <math.h>
#include <Library/LunchboxCommon.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#define AIN     BIT0                    // Analog Input at P1.0

volatile float displayLeds = 0;
volatile char number = 1;

/**
 * @brief
 * These settings are w.r.t. enabling ADC10 on LunchBox
 **/
void register_settings_for_ADC10()
{
    ADC10AE0 |= AIN;                            // P1.0 ADC option select
    ADC10CTL1 = INCH_0;                         // ADC Channel -> 1 (P1.0)
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON;  // Ref -> Vcc, 64 CLK S&H
}


/*@brief entry point for the code*/
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;                   // Stop watchdog timer


    BCSCTL1 |= (BIT0 + BIT1 + BIT2 + BIT3);     // Selecting RSELx as 15
    DCOCTL  |= (BIT6 + BIT5 + BIT4);            // Selecting DCOx as 7, DCO_freq = 15.6 MHz

    register_settings_for_ADC10();

    initialise_SerialPrint_on_lunchbox(); // a function
    __bis_SR_register(GIE);                     // Enable CPU Interrupt

    while(1)
    {
        ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
        while(ADC10CTL1 & ADC10BUSY);           // Wait for conversion to end

        int adcValue = ADC10MEM;

        if(adcValue != 0)
        {
            displayLeds = log10(adcValue);      // Taking Log base 10 of ADC value
            displayLeds = 2.0 * displayLeds;
        }

        number  = 1 * displayLeds;
        printf("(Log base 10) * 2 : %c \r\n", number + '0');

        int i;
        for (i = 0; i < 20000; i++);
    }
}
