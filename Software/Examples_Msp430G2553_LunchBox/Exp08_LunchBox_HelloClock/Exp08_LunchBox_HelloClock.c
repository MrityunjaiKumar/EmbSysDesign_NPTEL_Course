#include <msp430.h> 

#define LED BIT7                        // Led Pin

#define SW1 BIT3                        // 1.5Khz
#define SW2 BIT4                        // 3Khz
#define SW3 BIT5                        // 12Khz

volatile unsigned int i;                // Volatile to prevent removal

/**
 * @brief
 * Function to take input from 3 switches and change CPU clock accordingly
 **/
void switch_input()
{

    if(!(P1IN & SW1))                   // If SW1 is Pressed
    {
        __delay_cycles(2000);           // Wait 20ms to debounce
        while(!(P1IN & SW1));           // Wait till SW Released
        __delay_cycles(2000);           // Wait 20ms to debounce

        BCSCTL2 &=~ (BIT5 + BIT4);      //Reset VLO divider
        BCSCTL2 |= (BIT5 + BIT4);       //VLO = 12kHz/8 = 1.5kHz
    }


    if(!(P1IN & SW2))                   // If SW is Pressed
    {
        __delay_cycles(2000);           // Wait 20ms to debounce
        while(!(P1IN & SW2));           // Wait till SW Released
        __delay_cycles(2000);           // Wait 20ms to debounce

        BCSCTL2 &=~ (BIT5 + BIT4);      //Reset VLO divider
        BCSCTL2 |= (BIT4);              //VLO = 12kHz/4 = 3kHz
    }

    if(!(P1IN & SW3))                   // If SW is Pressed
    {
        __delay_cycles(2000);           // Wait 20ms to debounce
        while(!(P1IN & SW3));           // Wait till SW Released
        __delay_cycles(2000);           // Wait 20ms to debounce

        BCSCTL2 &=~ (BIT5 + BIT4);      //VLO = 12kHz/1 = 12kHz
    }

}

/**
 * @brief
 * These settings are wrt enabling GPIO on Lunchbox
 **/
void register_settings_for_GPIO()
{
    P1DIR |= LED;                       //P1.7 is set as Output
    P1DIR &=~ (SW1 + SW2 + SW3);        //P1.3, P1.4, P1.5 are set as Input
}


/**
 * @brief
 * These settings are w.r.t enabling VLO  on Lunch Box
 **/
void register_settings_for_VLO()
{
   BCSCTL3 |= LFXT1S_2;                 // LFXT1 = VLO
   do{
       IFG1 &= ~OFIFG;                  // Clear oscillator fault flag
       for (i = 50000; i; i--);         // Delay
     } while (IFG1 & OFIFG);            // Test osc fault flag

   BCSCTL2 |= SELM_3;                   // MCLK = VLO
}

/**
 * main.c
 */
int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;           //! Stop Watchdog (Not recommended for code in production and devices working in field)

    register_settings_for_VLO();        //Register settings for VLO
    register_settings_for_GPIO();       //Register settings for GPIO

    while(1)
    {
        switch_input();                 //Switch Input

        P1OUT ^= LED;                   //LED Toggle
        for (i = 100; i > 0; i--);

    }
}
