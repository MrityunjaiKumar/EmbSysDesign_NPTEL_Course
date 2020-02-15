#include <msp430.h>
#include <inttypes.h>

volatile char count = 0;
/**
 * @brief
 * These settings are wrt enabling GPIO on Lunchbox
 **/
void register_settings_for_GPIO()
{
    P1DIR |= 0xFF;                          //P1.0 to P1.7 are set as Output
    P1OUT |= 0x00;                          //Initially they are set to logic zero
}

/**
 * @brief
 * These settings are wrt enabling TIMER0 on Lunchbox
 **/
void register_settings_for_TIMER0()
{
    CCTL0 = CCIE;                         // CCR0 interrupt enabled
    TACTL = TASSEL_2 + MC_1 + ID_3;       // SMCLK/8, upmode
    CCR0 =  65535;                      // 0.5 Hz
}

/*@brief entry point for the code*/
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;             //! Stop Watchdog (Not recommended for code in production and devices working in field)
  BCSCTL2 = DIVS_1;                     //Divider for SMCLK (SMCLK/2)
  register_settings_for_TIMER0();
  register_settings_for_GPIO();
  _BIS_SR(CPUOFF + GIE);                // Enter LPM0 w/ interrupt

}

/*@brief entry point for TIMER0 interrupt vector*/
#pragma vector= TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    count++;
    P1OUT = count;                      //Assign value of Count to PORT 1 to represent it as binary number
}
