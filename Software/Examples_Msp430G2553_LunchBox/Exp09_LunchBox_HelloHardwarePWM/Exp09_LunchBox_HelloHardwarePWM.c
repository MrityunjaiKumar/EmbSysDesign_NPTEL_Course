#include <msp430.h> 

#define SW  BIT3                            // Switch -> P1.3
#define GREEN   BIT6                        // Green LED -> P1.6

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
    CCR1 = 1;                               // Set TA0.1 PWM duty cycle
    TACTL = TASSEL_2 + MC_1 + ID_3;                // Timer Clock -> SMCLK, Mode -> Up Count
}

/*@brief entry point for the code*/
void main(void) {
    unsigned int state = 0;

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    P1DIR &= ~SW;                           // Set SW pin -> Input
    P1REN |= SW;                            // Enable Resistor for SW pin
    P1OUT |= SW;                            // Select Pull Up for SW pin

    register_settings_for_TIMER0();

    while(1)
    {
        if(!(P1IN & SW))                    // If SW is Pressed
            {
                __delay_cycles(20000);      // Wait 20ms to debounce
                while(!(P1IN & SW));        // Wait till SW Released
                if(state == 0)
                {
                    CCR1 = CCR1 + 1500;       // Increase Duty from min to max
                    if(CCR1 > 20000)
                        state = 1;
                }
                else if(state == 1)
                {
                    CCR1 = CCR1 - 1500;       // Increase Duty from min to max
                    if(CCR1 < 100)
                        state = 0;
                }
            }
    }
}
