#include <msp430.h>
#include <math.h>
#include <Library/LunchboxCommon.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#define AIN     BIT0                    // Analog Input at P1.0

volatile float logValues = 0;
volatile unsigned char number = 1;

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

    register_settings_for_ADC10();              // Register setting for ADC10

    initialise_SerialPrint_on_lunchbox();       // Function to initialize Serial on LunchBox using LunchboxCommon.h

    unsigned int i;
    while(1)
    {
        ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
        while(ADC10CTL1 & ADC10BUSY);           // Wait for conversion to end

        int adcValue = ADC10MEM;

        if(adcValue != 0)
        {
            logValues = log10(adcValue);      // Taking Log base 10 of ADC value
            logValues = 2.0 * logValues;
        }

        number  = 1 * logValues;
        printf("ADC value : %d, \t(Log base 10) * 2 : %c \r\n", adcValue, number + '0');

        for (i = 0; i < 20000; i++);
    }
}
