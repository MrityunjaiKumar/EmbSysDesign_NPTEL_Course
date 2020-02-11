#include <msp430.h> 

#define SW      BIT3                    // Switch -> P1.3
#define GREEN   BIT6                    // Green LED -> P1.6


#define SMCLK   0
#define ACLK    1

unsigned int clockSource = 1;

/**
 * @brief
 * These settings are wrt enabling TIMER0 on Lunchbox
 **/
void register_settings_for_TIMER0()
{
    P1DIR |= GREEN;                         // Green LED -> Output
    P1SEL |= GREEN;                         // Green LED -> Select Timer Output

    CCR0 = 20000;                             // Set Timer0 PWM Period
    CCTL1 = OUTMOD_7;                       // Set TA0.1 Waveform Mode - Clear on Compare, Set on Overflow
    CCR1 = 10000;                               // Set TA0.1 PWM duty cycle
}

/**
 * @brief
 * These settings are wrt enabling TIMER0 wrt SMCLK on Lunchbox
 **/
void register_settings_for_TIMER0_with_SMCLK()
{
   TACTL = TASSEL_2 + MC_1 + ID_3;                // Timer Clock -> SMCLK, Mode -> Up Count
}

/**
 * @brief
 * These settings are wrt enabling TIMER0 wrt ACLK on Lunchbox
 **/
void register_settings_for_TIMER0_with_ACLK()
{
    TACTL = TASSEL_1 + MC_1 + ID_3;                // Timer Clock -> SMCLK, Mode -> Up Count
}

/*@brief entry point for the code*/
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    P1DIR &= ~SW;                       // Set SW pin -> Input
    P1REN |= SW;                        // Enable Resistor for SW pin
    P1OUT |= SW;                        // Select Pull Up for SW pin

    P1IES &= ~SW;                       // Select Interrupt on Rising Edge
    P1IE |= SW;                         // Enable Interrupt on SW pin

    register_settings_for_TIMER0();

    while(1)
    {
        __bis_SR_register(LPM0_bits +GIE);             // Enable CPU Interrupt
        switch(clockSource)
        {
            case SMCLK: register_settings_for_TIMER0_with_SMCLK();
                        break;

            case ACLK: register_settings_for_TIMER0_with_ACLK();
                       break;
        }
    }
}

/*@brief entry point for switch interrupt vector*/
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    clockSource = !clockSource;
    __bic_SR_register_on_exit(LPM0_bits + GIE); // Exit LPM0 on return to main
    P1IFG &= ~SW;                       // Clear SW interrupt flag
}
