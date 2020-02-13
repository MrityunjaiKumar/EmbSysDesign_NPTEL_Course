#include <msp430.h>

#define GREEN   BIT6
#define AIN     BIT0

/**
 *@brief This function maps input range to the required range
 *@param long Input value to be mapped
 *@param long Input value range, minimum value
 *@param long Input value range, maximum value
 *@param long Output value range, minimum value
 *@param long Output value range, maximum value
 *@return Mapped output value
 **/
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

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
 * These settings are wrt enabling TIMER0 on Lunchbox
 **/
void register_settings_for_TIMER0()
{
    P1DIR |= GREEN;                             // Green LED -> Output
    P1SEL |= GREEN;                             // Green LED -> Select Timer Output

    CCR0 = 255;                                 // Set Timer0 PWM Period
    CCTL1 = OUTMOD_7;                           // Set TA0.1 Waveform Mode
    CCR1 = 1;                                   // Set TA0.1 PWM duty cycle
    TACTL = TASSEL_2 + MC_1;                    // Timer Clock -> SMCLK, Mode -> Up Count
}

/*@brief entry point for the code*/
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                   //! Stop Watchdog (Not recommended for code in production and devices working in field)

    register_settings_for_ADC10();

    register_settings_for_TIMER0();


    while(1)
    {
        ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start

        while(ADC10CTL1 & ADC10BUSY);           // Wait for conversion to end

        CCR1 = map(ADC10MEM, 0, 1024, 1, 255);
    }
}
