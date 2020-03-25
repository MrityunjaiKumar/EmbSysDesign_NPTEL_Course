#include <msp430.h> 

#define SW  BIT3                    // Switch -> P1.3 (On-board Switch, Pull-Up configuration)
#define LED BIT7                    // Red LED -> P1.7 (On-Board LED, Active High Configuration)

/*@brief entry point for the code*/
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;       //! Stop Watchdog (Not recommended for code in production and devices working in field)

    P1DIR |= LED;                   // Set LED pin -> Output

    P1DIR &= ~SW;                   // Set SW pin -> Input


    while(1)
    {
        if(!(P1IN & SW))            // If SW is Pressed
        {
            while(!(P1IN & SW));    // Wait till SW Released
            P1OUT ^= LED;           // Toggle LED
        }
    }
}
