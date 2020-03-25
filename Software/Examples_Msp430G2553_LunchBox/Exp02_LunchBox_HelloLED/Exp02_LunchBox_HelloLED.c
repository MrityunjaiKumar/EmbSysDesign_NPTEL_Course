#include <msp430.h>

/*@brief entry point for the code*/
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;       //! Stop Watchdog (Not recommended for code in production and devices working in field)

    //P1DIR |= BIT7;
    P1DIR |= 0x80;                  // Set P1.7 to output direction

    //P1OUT |= BIT7;
   //P1OUT |= 0x80;                  // Set P1.7 to HIGH voltage

    //P1OUT &=~ BIT7;
    P1OUT &=~ 0x80;                  // Set P1.7 to LOW voltage

    return 0;
}
