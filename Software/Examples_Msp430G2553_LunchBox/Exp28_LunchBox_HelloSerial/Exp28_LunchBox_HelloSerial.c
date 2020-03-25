#include <msp430.h>

/**
 * @brief
 * These settings are wrt enabling uart on Lunchbox
 **/
void register_settings_for_UART()
{
    P1SEL = BIT1 + BIT2;              // Select UART RX/TX function on P1.1,P1.2
    P1SEL2 = BIT1 + BIT2;

    UCA0CTL1 |= UCSSEL_1;               // UART Clock -> ACLK
    UCA0BR0 = 3;                        // Baud Rate Setting for 32kHz 9600
    UCA0BR1 = 0;                        // Baud Rate Setting for 32kHz 9600
    UCA0MCTL = UCBRF_0 + UCBRS_3;       // Modulation Setting for 32kHz 9600
    UCA0CTL1 &= ~UCSWRST;               // Initialize UART Module
    IE2 |= UCA0RXIE;                    // Enable RX interrupt
}

/*@brief entry point for the code*/
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;           //! Stop Watchdog (Not recommended for code in production and devices working in field)

    register_settings_for_UART();

    __bis_SR_register(LPM0_bits + GIE); // Enter LPM0, Enable Interrupt

}

/**
 * @brief
 * Interrupt Vector for UART on LunchBox
 **/
#pragma vector=USCIAB0RX_VECTOR         // UART RX Interrupt Vector
__interrupt void USCI0RX_ISR(void)
{
    while (!(IFG2&UCA0TXIFG));          // Check if TX is ongoing
    UCA0TXBUF = UCA0RXBUF + 1;          // TX -> Received Char + 1
}
