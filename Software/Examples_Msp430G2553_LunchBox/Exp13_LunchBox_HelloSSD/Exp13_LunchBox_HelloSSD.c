#include <msp430.h>

#define SW      BIT3                    // Switch -> P2.3

// Define Pin Mapping of 7-segment Display
// Segments are connected to P1.0 - P1.7
#define SEG_A   BIT0
#define SEG_B   BIT1
#define SEG_C   BIT2
#define SEG_D   BIT3
#define SEG_E   BIT4
#define SEG_F   BIT5
#define SEG_G   BIT6
#define SEG_DP  BIT7

// Define each digit according to truth table
#define D0  (SEG_A + SEG_B + SEG_C + SEG_D + SEG_E + SEG_F)
#define D1  (SEG_B + SEG_C)
#define D2  (SEG_A + SEG_B + SEG_D + SEG_E + SEG_G)
#define D3  (SEG_A + SEG_B + SEG_C + SEG_D + SEG_G)
#define D4  (SEG_B + SEG_C + SEG_F + SEG_G)
#define D5  (SEG_A + SEG_C + SEG_D + SEG_F + SEG_G)
#define D6  (SEG_A + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G)
#define D7  (SEG_A + SEG_B + SEG_C)
#define D8  (SEG_A + SEG_B + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G)
#define D9  (SEG_A + SEG_B + SEG_C + SEG_D + SEG_F + SEG_G)
#define DA  (SEG_A + SEG_B + SEG_C + SEG_E + SEG_F + SEG_G)
#define DB  (SEG_C + SEG_D + SEG_E + SEG_F + SEG_G)
#define DC  (SEG_A + SEG_D + SEG_E + SEG_F)
#define DD  (SEG_B + SEG_C + SEG_D + SEG_E + SEG_G)
#define DE  (SEG_A + SEG_D + SEG_E + SEG_F + SEG_G)
#define DF  (SEG_A + SEG_E + SEG_F + SEG_G)


// Define mask value for all digit segments except DP
#define DMASK   ~(SEG_A + SEG_B + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G)

// Store digits in array for display
const unsigned int digits[16] = {D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, DA, DB, DC, DD, DE, DF};

volatile unsigned int i = 0;

/*@brief entry point for the code*/
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           //! Stop Watch dog (Not recommended for code in production and devices working in field)

    // Initialize 7-segment pins as Output
    P1DIR |= (SEG_A + SEG_B + SEG_C + SEG_D + SEG_E+ SEG_F + SEG_G + SEG_DP);

    P2DIR &= ~SW;                       // Set SW pin -> Input

    while(1)
    {
        if(!(P2IN & SW))                // If SW is Pressed
        {
            __delay_cycles(20000);      //Delay to avoid Switch Bounce
            while(!(P2IN & SW));        // Wait till SW Released
            i++;                        //Increment count
            if(i>15)
            {
                i=0;
            }
        }
        P1OUT = (P1OUT & DMASK) + digits[i];    // Display current digit
    }
}
