#include <msp430.h> 

#define RED   BIT7                    // Red LED -> P1.7

/**
 *@brief  This function provides delay
 *@param  unsigned int
 *@return void
 **/
void delay(unsigned int t)          // Custom delay function
{
    unsigned int i;
    for(i = t; i > 0; i--)
        __delay_cycles(1);              // __delay_cycles accepts only constants !
}

/*@brief entry point for the code*/
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    P1DIR |= RED;                       // Set LED pin -> Output

    while(1)
        {
            unsigned int j;
            for(j = 1; j < 500; j++)    // Increasing Intensity
            {
                P1OUT |= RED;           // LED ON
                delay(j);               // Delay for ON Time
                P1OUT &= ~RED;          // LED OFF
                delay(500-j);           // OFF Time = Period - ON Time
            }
            for(j = 500; j > 1; j--)    // Decreasing Intensity
            {
                P1OUT |= RED;           // LED ON
                delay(j);               // Delay for ON Time
                P1OUT &= ~RED;          // LED OFF
                delay(500-j);           // OFF Time = Period - ON Time
            }
        }
}
