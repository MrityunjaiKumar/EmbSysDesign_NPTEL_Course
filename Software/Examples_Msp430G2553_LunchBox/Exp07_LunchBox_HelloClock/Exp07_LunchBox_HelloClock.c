#include <msp430.h> 

#define LED BIT7

#define SW1 BIT3
#define SW2 BIT3
#define SW3 BIT3

volatile unsigned int i;                  // Volatile to prevent removal

/**
 * @brief
 * Function to take input from 3 switches and change CPU clock accordingly
 **/
void switch_input()
{
/*
    if(!(P1IN & SW1))            // If SW is Pressed
    {
        __delay_cycles(20000);  // Wait 20ms to debounce
        while(!(P1IN & SW1));    // Wait till SW Released
        __delay_cycles(20000);  // Wait 20ms to debounce
        // 12Khz VLO

        BCSCTL3 |= LFXT1S_2;                      // LFXT1 = VLO
        IFG1 &= ~OFIFG;                           // Clear OSCFault flag
        __bis_SR_register(SCG1 + SCG0);           // Stop DCO
        BCSCTL2 |= SELM_3;                        // MCLK = LFXT1
    }
*/

    if(!(P1IN & SW2))            // If SW is Pressed
    {
        __delay_cycles(20000);  // Wait 20ms to debounce
        while(!(P1IN & SW2));    // Wait till SW Released
        __delay_cycles(20000);  // Wait 20ms to debounce

        // 32Khz External

        // Loop until 32kHz crystal stabilizes
        do
          {
            IFG1 &= ~OFIFG;                         // Clear oscillator fault flag
            for (i = 50000; i; i--);                // Delay
          }
        while (IFG1 & OFIFG);                     // Test osc fault flag

        __bis_SR_register(SCG1 + SCG0);           // Stop DCO
        BCSCTL2 |= SELM_2;                        // MCLK = LFXT1
    }
/*
    if(!(P1IN & SW3))            // If SW is Pressed
    {

        __delay_cycles(2000);  // Wait 20ms to debounce
        while(!(P1IN & SW3));    // Wait till SW Released
        __delay_cycles(2000);  // Wait 20ms to debounce
        // DCO

        DCOCTL = 0;
        BCSCTL1 = 0;
        BCSCTL2 |= SELM_0;                        // MCLK = LFXT1
        __delay_cycles(2000);  // Wait 20ms to debounce

    }
*/
}

/**
 * main.c
 */
int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    do
              {
                IFG1 &= ~OFIFG;                         // Clear oscillator fault flag
                for (i = 50000; i; i--);                // Delay
              }
            while (IFG1 & OFIFG);                     // Test osc fault flag

            __bis_SR_register(SCG1 + SCG0);           // Stop DCO
            BCSCTL2 |= (BIT5 + BIT4);
            BCSCTL2 |= SELM_2;                        // MCLK = LFXT1


            BCSCTL2 &=~ (BIT5 + BIT4);
                        BCSCTL2 |= SELM_2;                        // MCLK = LFXT1
    /*
    DCOCTL = 0;
                BCSCTL1 = 0;
                BCSCTL2 |= SELM_0;                        // MCLK = LFXT1
                */
    // 12Khz VLO
/*
    BCSCTL3 |= LFXT1S_2;                      // LFXT1 = VLO
    // Loop until 32kHz crystal stabilizes
        do
          {
            IFG1 &= ~OFIFG;                         // Clear oscillator fault flag
            for (i = 50000; i; i--);                // Delay
          }
        while (IFG1 & OFIFG);                     // Test osc fault flag
    //__bis_SR_register(SCG1 + SCG0);           // Stop DCO
    BCSCTL2 |= SELM_3;                        // MCLK = LFXT1
*/
/*    DCOCTL = 0;
                    BCSCTL1 = 0;
                    BCSCTL2 |= SELM_0;                        // MCLK = LFXT1
/*
                    DCOCTL |= (BIT5 + BIT6);
                                        BCSCTL1 |= (BIT0 + BIT1 + BIT2);
                                        BCSCTL2 |= SELM_0;                        // MCLK = LFXT1
*/
    P1DIR |= LED;
    P1DIR &=~ (SW1 + SW2 + SW3);


    while(1)
    {

        switch_input();

        P1OUT |= LED;
        for (i = 500; i > 0; i--);

        switch_input();

        P1OUT &= ~LED;
        for (i = 500; i > 0; i--);
    }
}
