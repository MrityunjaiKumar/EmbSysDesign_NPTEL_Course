#include <msp430.h>
/**
 * @brief
 * These settings are wrt enabling GPIO on Lunchbox
 **/
void register_settings_for_GPIO()
{
    P1DIR |= BIT7;                            // P1.7 as Output
    P1OUT &= ~BIT7;                           // Initialize with zero
}
/**
 * @brief
 * These settings are w.r.t enabling TIMER0 on Lunch Box
 **/
void register_settings_for_TIMER0()
{
    CCTL0 = CCIE;                         // CCR0 interrupt enabled
    TACTL = TASSEL_1 + MC_1;              // ACLK = 32768 Hz, Up mode
    CCR0 =  32768;                        // 1 Hz
}
/*@brief entry point for the code*/
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;             //! Stop Watch dog (Not recommended for code in production and devices working in field)
  unsigned int i;

  do{
         IFG1 &= ~OFIFG;                    // Clear oscillator fault flag
         for (i = 50000; i; i--);           // Delay
    } while (IFG1 & OFIFG);                 // Test osc fault flag

  register_settings_for_TIMER0();
  register_settings_for_GPIO();
  _BIS_SR(LPM3_bits + GIE);                // Enter LPM3 w/ interrupt
}
/*@brief entry point for TIMER0 interrupt vector*/
#pragma vector= TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    P1OUT |= BIT7;                      //  LED HIGH
    __delay_cycles(200);
    P1OUT &=~ BIT7;
}
