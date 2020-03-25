#include <msp430.h>
#include <inttypes.h>

volatile char count = 0;
volatile unsigned int i;

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
 * These settings are w.r.t enabling TIMER0 on Lunch Box
 **/
void register_settings_for_TIMER0()
{
    CCTL0 = CCIE;                           // CCR0 interrupt enabled
    TACTL = TASSEL_1 + MC_1;                // ACLK = 32768 Hz, upmode
    CCR0 =  32768;                          // 1 Hz
}

/*@brief entry point for the code*/
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;             //! Stop Watch dog (Not recommended for code in production and devices working in field)

  do{
         IFG1 &= ~OFIFG;                // Clear oscillator fault flag
         for (i = 50000; i; i--);       // Delay
    } while (IFG1 & OFIFG);             // Test osc fault flag

  register_settings_for_TIMER0();
  register_settings_for_GPIO();
  _BIS_SR(LPM3_bits + GIE);             // Enter LPM3 w/ interrupt
}

/*@brief entry point for TIMER0 interrupt vector*/
#pragma vector= TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    count++;
    P1OUT = count;                      //Assign value of Count to PORT 1 to represent it as binary number
}
