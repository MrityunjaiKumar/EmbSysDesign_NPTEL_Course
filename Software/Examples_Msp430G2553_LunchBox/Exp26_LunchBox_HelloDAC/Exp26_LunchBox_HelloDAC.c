#include <msp430.h>
#include <inttypes.h>

char count = 0;
unsigned int i;

/**
 * @brief
 * These settings are w.r.t enabling GPIO on LunchBox
 **/
void register_settings_for_GPIO()
{
    P1DIR |= 0xFF;                                //P1.0 to P1.7 are set as Output
    P1OUT &= ~(0xFF);                             //Initially they are set to logic zero
}

/*@brief entry point for the code*/
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 //! Stop Watch dog (Not recommended for code in production and devices working in field)

  register_settings_for_GPIO();

  while(1)
  {
      P1OUT = count;                        // Assign value of Count to PORT 1 to represent it as binary number
      for(i = 0; i < 5000; i++);
      count ++;                             // Increment count
  }
}
