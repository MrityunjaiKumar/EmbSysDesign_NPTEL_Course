#include <msp430.h> 
#include <inttypes.h>

#define SW      BIT3                    // Switch -> P1.3
#define RED     BIT4                    // Red LED -> P1.4
#define GREEN   BIT5                    // Green LED -> P1.5
#define BLUE    BIT6                    // Blue LED -> P1.6

uint32_t millis = 0, lastCount = 0;
uint8_t count = 0;

/**
 * @brief
 * These settings are wrt enabling GPIO on Lunchbox
 **/
void register_settings_for_GPIO()
{
    P1DIR |= RED + GREEN + BLUE;          // P1.4, P1.5 and P1.6 pins output the rest are input
    P1OUT &=~ RED + GREEN + BLUE;         // Shut down each LED

    P1REN |= SW;                          // Enable internal pull-up/down resistors
    P1OUT |= SW;                          //Select pull-up mode for P1.3
    P1IE |= SW;                           // P1.3 interrupt enabled
    P1IES |= SW;                          // P1.3 Hi/lo edge
    P1IFG &= ~SW;                         // P1.3 IFG cleared
}

/**
 * @brief
 * These settings are wrt enabling TIMER0 on Lunchbox
 **/
void register_settings_for_TIMER0()
{
    CCTL0 = CCIE;                         // CCR0 interrupt enabled
    TACTL = TASSEL_2 + MC_1 + ID_0;       // SMCLK/1, upmode
    CCR0 =  497;                          // 1 kHz
}

/*@brief entry point for the code*/
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;             //! Stop Watchdog (Not recommended for code in production and devices working in field)

  register_settings_for_TIMER0();

  register_settings_for_GPIO();

  _BIS_SR(CPUOFF + GIE);                // Enter LPM0 w/ interrupt

}

/*@brief entry point for TIMER0 interrupt vector*/
#pragma vector= TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
   millis++;
}

/*@brief entry point for switch interrupt vector*/
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
   if(count == 0)
   {
       lastCount = millis;
       count = 1;
       P1IES &=~ SW;
   }
   else if(count == 1)
   {
       count = 0;
       P1IES |= SW;
       if((millis - lastCount) < 1000)
           P1OUT ^= BIT4;                      // Toggle P1.4 if switch is pressed for less than 1 seconds
       else if((millis - lastCount) < 2000)
           P1OUT ^= BIT5;                      // Toggle P1.5 if switch is pressed for less than 2 seconds
       else if((millis - lastCount) < 3000)
           P1OUT ^= BIT6;                      // Toggle P1.6 if switch is pressed for less than 3 seconds
   }

   P1IFG &= ~SW;                               // P1.3 IFG cleared
}
