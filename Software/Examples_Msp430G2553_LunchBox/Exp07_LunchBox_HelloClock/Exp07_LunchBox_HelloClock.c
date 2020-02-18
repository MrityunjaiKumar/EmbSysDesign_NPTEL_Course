#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    volatile unsigned int i;                  // Volatile to prevent removal
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // 12Khz VLO
/*
    BCSCTL3 |= LFXT1S_2;                      // LFXT1 = VLO
    IFG1 &= ~OFIFG;                           // Clear OSCFault flag
    __bis_SR_register(SCG1 + SCG0);           // Stop DCO
    BCSCTL2 |= SELM_3;                        // MCLK = LFXT1
*/

    // 32Khz External

    //BCSCTL3 |= LFXT1S_0;                      // LFXT1 = External
    // Loop until 32kHz crystal stabilizes
    do
      {
        IFG1 &= ~OFIFG;                         // Clear oscillator fault flag
        for (i = 50000; i; i--);                // Delay
      }
      while (IFG1 & OFIFG);                     // Test osc fault flag
    __bis_SR_register(SCG1 + SCG0);           // Stop DCO
    BCSCTL2 |= SELM_2;                        // MCLK = LFXT1

    // DCO
    /*
    DCOCTL = 0;
    BCSCTL1 = 0;
*/

    P1DIR = 0xFF;                             // All P1.x outputs
    P1OUT = 0;                                // All P1.x reset
    P2DIR = 0xFF;                             // All P2.x outputs
    P2OUT = 0;                                // All P2.x reset
    P1SEL |= BIT0;
    for (;;)
    {
        P1OUT |= BIT7;                          // P1.0 set
        for (i = 500; i > 0; i--);               // Delay 1x
        P1OUT &= ~BIT7;                         // P1.0 reset
        for (i = 500; i > 0; i--);             // Delay 100x
    }
}
