#include <msp430.h> 

#define SW  BIT3                    // Switch -> P1.3
#define LED BIT7                    // Red LED -> P1.7

/*@brief entry point for the code*/
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer

    P1DIR |= LED;                   // Set LED pin -> Output

    P1DIR &= ~SW;                   // Set SW pin -> Input
    P1REN |= SW;                    // Enable Resistor for SW pin
    P1OUT |= SW;                    // Select Pull Up for SW pin

    while(1)
    {
        if(!(P1IN & SW))            // If SW is Pressed
        {
            P1OUT ^= LED;           // Toggle LED
        }
    }
}
