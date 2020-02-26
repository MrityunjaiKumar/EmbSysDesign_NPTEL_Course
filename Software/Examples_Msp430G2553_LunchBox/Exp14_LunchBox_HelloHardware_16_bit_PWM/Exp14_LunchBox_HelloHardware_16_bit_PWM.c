#include <msp430.h> 

#define GREEN   BIT6                        // Green LED -> P1.6

/**
 * @brief
 * These settings are w.r.t enabling TIMER0 on Lunchbox
 **/
void register_settings_for_TIMER0()
{
    P1DIR |= GREEN;                         // Green LED -> Output
    P1SEL |= GREEN;                         // Green LED -> Select Timer Output

    CCR0 = 65535;                             // Set Timer0 PWM Period
    CCTL1 = OUTMOD_7;                       // Set TA0.1 Waveform Mode - Clear on Compare, Set on Overflow
    CCR1 = 0;                               // Set TA0.1 PWM duty cycle
    CCTL0 = CCIE;                           // CCR0 Enable Interrupt
    TACTL = TASSEL_2 + MC_1;                // Timer Clock -> SMCLK, Mode -> Up Count
}

/**
 * @brief
 * Entry point for the code
 **/
void main(void) {

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    BCSCTL1 |= (BIT0 + BIT1 + BIT2 + BIT3); // Selecting RSELx as 15
    DCOCTL  |= (BIT6 + BIT5 + BIT4);        // Selecting DCOx as 7, DCO_freq = 15.6 MHz, Room Temp. ~ 25 deg. Celsius, Operating voltage 3.3 V

    register_settings_for_TIMER0();

    __bis_SR_register(GIE);                 // Enable CPU Interrupt

    while(1)
    {
    }

}

/**
 * @brief
 * Entry point for TIMER0_interrupt vector
 **/
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    CCR1 = CCR1 + 1;                        // Increment CCR1
}
