//***************************************************************************************
//  MSP430 Turn ON the LED Demo - Software ON P1.0
//
//  Description; Turn ON P1.0 by making the Pin HIGH.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  Jan 2020
//  Built for LunchBox
//***************************************************************************************

#include <msp430.h>				

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		//! Stop Watchdog (Not recommended for code in production and devices working in field)
	P1DIR |= 0x01;					// Set P1.0 to output direction

	P1OUT |= 0x01;				    // Toggle P1.0 using exclusive-OR

	return 0;
}
