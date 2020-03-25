#include <msp430.h>
#include <math.h>

#define AIN     BIT0                    // Analog Input at P1.0

#define P1      BIT0                    // Charlieplex P1 -> P2.0
#define P2      BIT1                    // Charlieplex P2 -> P2.1
#define P3      BIT2                    // Charlieplex P3 -> P2.2

volatile float logValues = 0;
volatile char number = 1;

//Data Table for 12 Charliplexed LEDs
const unsigned int hi[12] = {P2,P1,P3,P1,P3,P2};
const unsigned int lo[12] = {P1,P2,P1,P3,P2,P3};
const unsigned int z[12]  = {P3,P3,P2,P2,P1,P1};

/**
 * @brief
 * These settings are w.r.t. Charlieplexed LEDs
 **/
void charlie(unsigned int value)
{
    P2DIR &= ~(z[value]);                // Set high-Z pins as Input
    P2DIR |= (lo[value] + hi[value]);    // Set high & low pins as Output
    P2OUT &= ~lo[value];                 // Set state of low pin
    P2OUT |= hi[value];                  // Set state of high pin
}

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

/**
 * @brief
 * These settings are w.r.t. enabling TIMER0 on Lunch Box
 **/
void register_settings_for_TIMER0()
{
    CCTL0 = CCIE;                               // CCR0 interrupt enabled
    TACTL = TASSEL_1 + MC_1;                    // ACLK = 32768 Hz, up mode
    CCR0 =  32;                                 // 1024 Hz
}

/*@brief entry point for the code*/
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;                   // Stop watchdog timer


    BCSCTL1 |= (BIT0 + BIT1 + BIT2 + BIT3);     // Selecting RSELx as 15
    DCOCTL  |= (BIT6 + BIT5 + BIT4);            // Selecting DCOx as 7, DCO_freq = 15.6 MHz

    register_settings_for_ADC10();
    register_settings_for_TIMER0();

    __bis_SR_register(GIE);                     // Enable CPU Interrupt

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
    }
}

/*@brief entry point for TIMER0 interrupt vector*/
#pragma vector= TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    if(number <= logValues)
    {
        charlie(number-1);                      //Switch on LED (i)
        number++;
    }
    else
    {
        number = 1;
    }
}
