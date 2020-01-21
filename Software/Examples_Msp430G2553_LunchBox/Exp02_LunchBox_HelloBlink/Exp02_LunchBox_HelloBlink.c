#include <msp430.h> 



/*@brief entry point for the code*/
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	//! Stop Watchdog (Not recommended for code in production and devices working in field)

	P1DIR |= BIT7;            // P1.0 (Red LED)

    while(1)
    {
        volatile unsigned long i;

        P1OUT |= BIT7;              //Red LED -> ON
        for(i = 0; i<10000; i++);   //delay

        P1OUT &= ~BIT7;             //Red LED -> OFF
        for(i = 0; i<10000; i++);   //delay
    }
}
