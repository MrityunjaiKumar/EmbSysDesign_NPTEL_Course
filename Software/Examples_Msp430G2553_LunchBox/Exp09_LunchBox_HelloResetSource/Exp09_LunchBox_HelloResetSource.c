#include <msp430.h>

#define  LED1 BIT0                                  // LED to indicate that main() has started
#define  LED2 BIT1                                  // LED to indicate that program is executing
#define  LEDA BIT2                                  // To indicate Power On Reset
#define  LEDB BIT3                                  // To indicate Watch dog Reset
#define  LEDC BIT4                                  // To indicate External Reset (by reset switch)

/**
 * @brief
 * These settings are wrt enabling GPIO on Lunchbox
 **/
void register_settings_for_GPIO()
{
    P1DIR |= (LED1 + LED2 + LEDA + LEDB + LEDC);    // P1.0 to P1.4 as output

    P1OUT |= LED1;                                  // LED1 representing main() is set

    P1OUT &=~ (LED2 + LEDA + LEDB + LEDC);          // Turning all other LEDs OFF
}

/**
 * @brief
 * These settings are w.r.t check source of reset on Lunch Box
 **/
void checking_reset_source()
{
    if (IFG1 & PORIFG)                              // Check for Power on Reset flag (POR)
    {
        P1OUT|= LEDA;                               // Turning LEDA On
        P1OUT &= ~LEDB;                             // Turning LEDB Off
        P1OUT &= ~LEDC;                             // Turning LEDC Off

        /* Settings for Watchdog Timer register
                Giving Watchdog Timer Password
                Clearing Watchdog counter to 0000h
                Watchdog Source select --> ACLK
                Watchdog Timer interval set to generate watchdog reset at 2 secs
        */
        WDTCTL = (WDTPW + WDTCNTCL + WDTSSEL + WDTIS0);

        IFG1 &=~ PORIFG;                            // Clearing Power on Reset flag
    }

    else if (IFG1 & RSTIFG)                         // Check for External Reset flag (RST)
    {
        P1OUT|= LEDC;                               // Turning LEDC On
        P1OUT &= ~LEDA;                             // Turning LEDA Off
        P1OUT &= ~LEDB;                             // Turning LEDB Off

        /* Settings Watchdog Timer register
                Giving Watchdog Timer Password
                Clearing Watchdog counter to 0000h
                Watchdog Source select --> ACLK
                Watchdog Timer interval set to generate watchdog reset at 2 secs
        */
        WDTCTL = (WDTPW + WDTCNTCL + WDTSSEL + WDTIS0);

        IFG1 &=~ RSTIFG;                            // Clearing External Reset flag
    }

    else if (IFG1 & WDTIFG)                         // Check for Watch Dog Reset flag (WDT)
    {
        P1OUT|= LEDB;                               // Turning LEDB On
        P1OUT &= ~LEDA;                             // Turning LEDA Off
        P1OUT &= ~LEDC;                             // Turning LEDC Off

        IFG1 &=~ PORIFG;                            // Clearing Power on Reset flag
        IFG1 &=~ RSTIFG;                            // Clearing External Reset flag
    }

}

/*@brief entry point for the code*/
void main(void)
{
      WDTCTL = WDTPW | WDTHOLD;   //! Stop Watchdog (Not recommended for code in production and devices working in field)
      volatile unsigned int i;

      BCSCTL1 |= DIVA_3;                            // Dividing ACLK by 8, 32768Hz/8 = 4096Hz

      register_settings_for_GPIO();

      /* This loop checks for Oscillator fault flag to reset means
         it delays execution until external crystal is Power On  */

      do
      {
         IFG1 &= ~OFIFG;                            // Clear oscillator fault flag
         for (i = 10000; i ; i--);                  // Delay, minimum value of i = 5000
      }  while (IFG1 & OFIFG);                      // Test osc fault flag

      checking_reset_source();

      while(1)
      {
          P1OUT ^= LED2;                            // Toggle LED
          __delay_cycles(1000000);
      }
}
