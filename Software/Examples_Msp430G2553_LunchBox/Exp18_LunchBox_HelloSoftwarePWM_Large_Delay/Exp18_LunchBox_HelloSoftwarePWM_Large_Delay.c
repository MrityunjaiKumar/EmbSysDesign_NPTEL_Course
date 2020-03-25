#include <msp430.h> 

#define RED   BIT7                          // Red LED -> P1.7

/**
 *@brief  This function provides delay
 *@param  unsigned int
 *@return void
 **/
void delay(unsigned int t)                  // Custom delay function
{
    unsigned int i;
    for(i = t; i > 0; i--)
        __delay_cycles(50);                 // __delay_cycles accepts only constants !
}

/*@brief entry point for the code*/
void main(void) {
    unsigned int j;
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    P1DIR |= RED;                           // Set LED pin -> Output

    while(1)
        {

            for(j = 0; j < 256; j++)        // Increasing Intensity
            {
                P1OUT |= RED;               // LED ON
                if (j !=0) delay(j);        // Delay for ON Time
                P1OUT &= ~RED;              // LED OFF
                if ((255-j)!=0) delay(255-j);     // OFF Time = Period - ON Time
            }
            for(j = 255; j > 0; j--)        // Decreasing Intensity
            {
                P1OUT |= RED;               // LED ON
                if (j !=0) delay(j);        // Delay for ON Time
                P1OUT &= ~RED;              // LED OFF
                if ((255-j) != 0 ) delay(255-j); // OFF Time = Period - ON Time
            }
        }
}
