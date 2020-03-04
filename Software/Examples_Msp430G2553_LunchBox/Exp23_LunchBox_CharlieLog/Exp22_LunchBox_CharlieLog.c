#include <msp430.h>
#include <math.h>

#define AIN     BIT0

#define POUT    P2OUT                   // PxOUT for Charlieplexed LEDs
#define PDIR    P2DIR                   // PxDIR for Charlieplexed LEDs
#define P1      BIT0                    // Charlieplex P1 -> P2.0
#define P2      BIT1                    // Charlieplex P2 -> P2.1
#define P3      BIT2                    // Charlieplex P3 -> P2.2

#define SW      BIT3                    // Switch -> P1.3

volatile float displayLeds = 0;
volatile char number = 0;

//Data Table for 12 Charliplexed LEDs
const unsigned int hi[12] = {P1,P2,P3,P2,P3,P1};
const unsigned int lo[12] = {P2,P1,P1,P3,P2,P3};
const unsigned int z[12]  = {P3,P3,P2,P1,P1,P2};

void charlie(unsigned int value)
{
    PDIR &= ~(z[value]);   // Set high-Z pins as Input
    PDIR |= (lo[value] + hi[value]);    // Set high & low pins as Output
    POUT &= ~lo[value];                 // Set state of low pin
    POUT |= hi[value];                  // Set state of high pin
}


volatile unsigned int leds[6]= {7,7,7,7,7,7};    // Global Array of LEDs


/**
 * @brief
 * These settings are wrt enabling ADC10 on Lunchbox
 **/
void register_settings_for_ADC10()
{
    ADC10AE0 |= AIN;                            // P1.0 ADC option select
    ADC10CTL1 = INCH_0;                         // ADC Channel -> 1 (P1.0)
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON;  // Ref -> Vcc, 64 CLK S&H
}

/**
 * @brief
 * These settings are w.r.t enabling TIMER0 on Lunch Box
 **/
void register_settings_for_TIMER0()
{
    CCTL0 = CCIE;                         // CCR0 interrupt enabled
    TACTL = TASSEL_1 + MC_1;              // ACLK = 32768 Hz, upmode
    CCR0 =  250;                        // 1 Hz
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer


    BCSCTL1 |= (BIT0 + BIT1 + BIT2 + BIT3); // Selecting RSELx as 15
            DCOCTL  |= (BIT6 + BIT5 + BIT4);        // Selecting DCOx as 7, DCO_freq = 15.6 MHz, Room Temp. ~ 25 deg. Celsius, Operating voltage 3.3 V
    register_settings_for_ADC10();
    register_settings_for_TIMER0();

    P1DIR |= BIT2;
    __bis_SR_register(GIE);                     // Enable CPU Interrupt

    while(1)
    {
            ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
            while(ADC10CTL1 & ADC10BUSY);           // Wait for conversion to end

            int adcValue = ADC10MEM;
            if(adcValue != 0)
            {
                displayLeds = log10(adcValue);
                displayLeds = 2.0 * displayLeds;
            }
    }
}

/*@brief entry point for TIMER0 interrupt vector*/
#pragma vector= TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    if(number <= displayLeds)
    {
        leds[number] = number;             // Switch on LED (i)
    }
    else
    {
        leds[number] = 7;
    }
    charlie(leds[number]);
    number++;
    if(number == 6)
        number = 0;

}
