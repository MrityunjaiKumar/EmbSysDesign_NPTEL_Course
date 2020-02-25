#include <msp430.h> 

#define GREEN   BIT6                        // Green LED -> P1.6

/**
 * @brief
 * These settings are wrt enabling TIMER0 on Lunchbox
 **/
void register_settings_for_TIMER0()
{
    P1DIR |= GREEN;                         // Green LED -> Output
    P1SEL |= GREEN;                         // Green LED -> Select Timer Output

    CCR0 = 4000;                             // Set Timer0 PWM Period
    CCTL1 = OUTMOD_7;                       // Set TA0.1 Waveform Mode - Clear on Compare, Set on Overflow
    CCR1 = 200;                               // Set TA0.1 PWM duty cycle
    TACTL = TASSEL_2 + MC_1 + ID_3;                // Timer Clock -> SMCLK, Mode -> Up Count
}

/*@brief entry point for the code*/
void main(void) {

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    register_settings_for_TIMER0();
    __bis_SR_register(LPM1_bits + GIE); // Enter LPM4 and Enable CPU Interrupt
}
