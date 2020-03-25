#include <msp430.h> 

#define SW      BIT3                    // Switch -> P1.3
#define RED     BIT7                    // Red LED -> P1.7

/*@brief entry point for the code*/
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           //! Stop Watchdog (Not recommended for code in production and devices working in field)

    P1DIR |= RED;                       // Set LED pin -> Output
    P1DIR &= ~SW;                       // Set SW pin -> Input
    P1REN |= SW;                        // Enable Resistor for SW pin
    P1OUT |= SW;                        // Select Pull Up for SW pin

    P1IES &= ~SW;                       // Select Interrupt on Rising Edge
    P1IE |= SW;                         // Enable Interrupt on SW pin

    __bis_SR_register(GIE);             // Enable CPU Interrupt

    while(1);
}

/*@brief entry point for switch interrupt*/
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(P1IFG & SW)                      // If SW is Pressed
    {
        P1OUT ^= RED;                   // Toggle RED LED
        volatile unsigned long i;
        for(i = 0; i<10000; i++);       //delay
        P1IFG &= ~SW;                   // Clear SW interrupt flag
    }

}
