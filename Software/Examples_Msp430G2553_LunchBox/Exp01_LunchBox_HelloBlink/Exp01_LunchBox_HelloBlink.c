#include <msp430.h>

/*@brief entry point for the code*/
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;       //! Stop Watchdog (Not recommended for code in production and devices working in field)
    P1DIR |= 0x01;                  // Set P1.0 to output direction

    P1OUT |= 0x01;                  // Toggle P1.0 using exclusive-OR

    return 0;
}
