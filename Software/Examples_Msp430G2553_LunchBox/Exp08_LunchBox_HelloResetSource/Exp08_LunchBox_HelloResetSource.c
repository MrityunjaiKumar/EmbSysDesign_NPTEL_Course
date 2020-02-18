#include <msp430.h>

#define  LED1 BIT1
#define  LEDA BIT2
#define  LEDB BIT3
#define  LEDC BIT4

void main(void)
{
    BCSCTL1 |= DIVA_3;
    WDTCTL = WDTPW | WDTSSEL;
  P1DIR |= (LED1 + LEDA + LEDB + LEDC);
  P1OUT &=~ (LED1 + LEDA + LEDB + LEDC);
  if (IFG1 & WDTIFG)
          {
              P1OUT|= LEDB;
              P1OUT &= ~LEDA;
              P1OUT &= ~LEDC;
              IFG1 &= ~ WDTIFG;
          }
  else if (IFG1 & PORIFG)
      {
          P1OUT|= LEDA;
          P1OUT &= ~LEDB;
          P1OUT &= ~LEDC;
      }
  else if (IFG1 & RSTIFG)
          {
              P1OUT|= LEDC;
              P1OUT &= ~LEDA;
              P1OUT &= ~LEDB;
          }
  while(1)
  {
      P1OUT ^= LED1;
      __delay_cycles(1000000);
  }
}
