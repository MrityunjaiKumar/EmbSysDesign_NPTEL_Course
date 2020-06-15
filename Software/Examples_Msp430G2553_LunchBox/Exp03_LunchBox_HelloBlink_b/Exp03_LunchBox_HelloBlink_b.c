//#include <msp430.h>
/*@brief entry point for the code*/
void main(void)
{
//	WDTCTL = WDTPW | WDTHOLD;
    (*(volatile unsigned int *) 0x0120) = 0x5A00 | 0x0080;
    /* ! Stop Watchdog (Not recommended for code in production
     and devices working in field)
     */

    //P1DIR |= BIT7;
    (*(volatile unsigned char *) 0x0022) |= 0x80;
    // P1.7 (Red LED)

    volatile unsigned long i;
    while (1)
    {

        //P1OUT |= BIT7;
//        P1OUT |= 0x80;              //Red LED -> ON
        (*(volatile unsigned char *) 0x0021) |= 0x80;
        //Red LED -> ON
        for (i = 0; i < 10000; i++)
            ;   //delay

        //P1OUT &=~ BIT7;
        //P1OUT &= ~0x80;             //Red LED -> OFF
        (*(volatile unsigned char *) 0x0021) &= ~0x80;
        for (i = 0; i < 10000; i++)
            ;   //delay
    }
}
