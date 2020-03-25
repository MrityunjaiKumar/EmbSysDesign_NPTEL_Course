#include <msp430.h>
#include <inttypes.h>
#include<stdio.h>

#define RED  BIT7                    // Switch -> P1.3 (On-board Switch, Pull-Up configuration)
#define AIN         BIT0        // Channel A0

/**
 *@brief Delay function for producing delay in 0.1 ms increments
 *@param t milliseconds to be delayed
 *@return void
 **/
void delay(uint16_t t)
{
    uint16_t i;
    for(i=t; i > 0; i--)
        __delay_cycles(100);
}


/**
 * @brief
 * These settings are wrt enabling ADC10 on Lunchbox
 **/
void register_settings_for_ADC10()
{
    ADC10AE0 |= AIN;                            // P1.0 ADC option select
    ADC10CTL1 = INCH_0;                         // ADC Channel -> 1 (P1.0)
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON;  // Ref -> Vcc, 64 CLK S&H , ADC - ON
}

/*@brief entry point for the code*/
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                       //! Stop Watchdog (Not recommended for code in production and devices working in field)

    P1DIR |= RED;            // P1.7 (Red LED)

    register_settings_for_ADC10();

    while(1)
    {
        ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start

        while(ADC10CTL1 & ADC10BUSY);           // Wait for conversion to end

        uint32_t lfsr = 0;
        lfsr |= ADC10MEM;
        uint32_t count = 0;

        while(count < 0xFFFFFFFF)
        {
            uint32_t bit;
            bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 6) ^ (lfsr >> 7));     // Taking taps
            lfsr = (lfsr >> 1) | (bit << 31);                                   // Shifting LFSR

            P1OUT = (lfsr & 0x00000001)<<7;  ;      // Setting RED LED according to one bit of 32*bit LFSR
            count = count + 1;
            delay(150);
        }
    }
}
