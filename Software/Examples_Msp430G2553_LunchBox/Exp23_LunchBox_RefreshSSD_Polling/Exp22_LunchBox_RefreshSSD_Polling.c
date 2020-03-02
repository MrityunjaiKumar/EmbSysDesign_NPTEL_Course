#include <msp430.h>
#include <inttypes.h>

#define AIN     BIT0

#define SW      BIT4                    // Switch -> P2.4

// Define Pin Mapping of 7-segment Display
// Segment A is connected to P2.5
// Segments B-G and DP are connected to P1.1 - P1.7
#define SEG_A   BIT5
#define SEG_B   BIT1
#define SEG_C   BIT2
#define SEG_D   BIT3
#define SEG_E   BIT4
#define SEG_F   BIT5
#define SEG_G   BIT6
#define SEG_DP  BIT7

// Segments 1-4 are connected to P2.0 - P2.3
#define SEG_1   BIT0
#define SEG_2   BIT1
#define SEG_3   BIT2
#define SEG_4   BIT3

volatile unsigned int displayValue = 0;
volatile float delayValue = 0;

/**
 *@brief Delay function for producing delay in .1 ms increments
 *@param t Input time to be delayed
 *@return void
 **/
void delay(uint16_t t)
{
    uint16_t i;
    for(i=t; i > 0; i--)
        __delay_cycles(15000);
}

/********************************
 *
 ********************************/
/**
 *@brief This function displays digit on single Seven Segment Display
 *@param digit Digit to be displayed
 *@return void
 **/
void digitToDisplay(unsigned int digit){

    switch(digit){
        case 0: P2OUT |= SEG_A;
                P1OUT |= SEG_B + SEG_C + SEG_D + SEG_E + SEG_F;
                break;

        case 1: P1OUT |= SEG_B + SEG_C;
                break;

        case 2: P2OUT |= SEG_A;
                P1OUT |= SEG_B + SEG_D + SEG_E + SEG_G;
                break;

        case 3: P2OUT |= SEG_A;
                P1OUT |= SEG_B + SEG_C + SEG_D + SEG_G;
                break;

        case 4: P1OUT |= SEG_B + SEG_C + SEG_F + SEG_G;
                break;

        case 5: P2OUT |= SEG_A;
                P1OUT |= SEG_C + SEG_D + SEG_F + SEG_G;
                break;

        case 6: P2OUT |= SEG_A;
                P1OUT |= SEG_C + SEG_D + SEG_E + SEG_F + SEG_G;
                break;

        case 7: P2OUT |= SEG_A;
                P1OUT |= SEG_B + SEG_C;
                break;

        case 8: P2OUT |= SEG_A;
                P1OUT |= SEG_B + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G;
                break;

        case 9: P2OUT |= SEG_A;
                P1OUT |= SEG_B + SEG_C + SEG_D + SEG_F + SEG_G;
                break;
  }
}

/**
 *@brief This function separates 4 digits from a number and displayed it on a four digit seven segment display
 *@param number Number to be displayed
 *@return Void
 **/
void fourDigitNumber(int number)
{
  unsigned int displayDigit[4];
  displayDigit[0] = number / 1000;
  number = number % 1000;
  displayDigit[1] = number / 100;
  number = number % 100;
  displayDigit[2] = number / 10;
  displayDigit[3] = number % 10;


  P1OUT &=~ (SEG_B + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G);
  P2OUT &=~ SEG_A;
  P2OUT &=~ (SEG_1 + SEG_2 + SEG_3 + SEG_4);

  digitToDisplay(displayDigit[0]);    // Display current digit
  P2OUT |= SEG_4;
  P1OUT &=~ SEG_DP;
  delay(delayValue);
  P1OUT ^= SEG_DP;

  P1OUT &=~ (SEG_B + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G);
  P2OUT &=~ SEG_A;
  P2OUT &=~ (SEG_1 + SEG_2 + SEG_3 + SEG_4);

  digitToDisplay(displayDigit[1]);    // Display current digit
  P2OUT |= SEG_3;
  delay(delayValue);

  P1OUT &=~ (SEG_B + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G);
  P2OUT &=~ SEG_A;
  P2OUT &=~ (SEG_1 + SEG_2 + SEG_3 + SEG_4);

  digitToDisplay(displayDigit[2]);    // Display current digit
  P2OUT |= SEG_2;
  delay(delayValue);

  P1OUT &=~ (SEG_B + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G);
  P2OUT &=~ SEG_A;
  P2OUT &=~ (SEG_1 + SEG_2 + SEG_3 + SEG_4);

  digitToDisplay(displayDigit[3]);    // Display current digit
  P2OUT |= SEG_1;
  delay(delayValue);
}

/**
 *@brief This function maps input range to the required range
 *@param long Input value to be mapped
 *@param long Input value range, minimum value
 *@param long Input value range, maximum value
 *@param long Output value range, minimum value
 *@param long Output value range, maximum value
 *@return Mapped output value
 **/
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * @brief
 * These settings are wrt enabling ADC10 on Lunchbox
 **/
void register_settings_for_ADC10()
{
    ADC10AE0 |= AIN;                            // P1.0 ADC option select
    ADC10CTL1 = INCH_0;                         // ADC Channel -> 1 (P1.0)
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON;  // Ref -> Vcc, 64 CLK S&H
}




/*@brief entry point for the code*/
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;                   //! Stop Watchdog (Not recommended for code in production and devices working in field)

    BCSCTL1 |= (BIT0 + BIT1 + BIT2 + BIT3); // Selecting RSELx as 15
        DCOCTL  |= (BIT6 + BIT5 + BIT4);        // Selecting DCOx as 7, DCO_freq = 15.6 MHz, Room Temp. ~ 25 deg. Celsius, Operating voltage 3.3 V
    P1DIR |= (SEG_B + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G + SEG_DP);
    P2DIR |= SEG_A;
    P2DIR |= (SEG_1 + SEG_2 + SEG_3 + SEG_4);
    P2DIR &=~ SW;

    P1OUT &=~ (SEG_B + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G);
    P2OUT &=~ SEG_A;
    P2OUT &=~ (SEG_1 + SEG_2 + SEG_3 + SEG_4);

    register_settings_for_ADC10();

    unsigned int i;

    while(1)
    {

        ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
        while(ADC10CTL1 & ADC10BUSY);           // Wait for conversion to end
        int adcValue1 = ADC10MEM;

        ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
        while(ADC10CTL1 & ADC10BUSY);           // Wait for conversion to end
        int adcValue2 = ADC10MEM;

        int avgADC = (adcValue1 + adcValue2) / 2;

        if (avgADC <500)
            delayValue = 1 + .1*avgADC;
        else
            delayValue = 51 + 0.8*(avgADC - 500);


        /*
        unsigned int count = map(ADC10MEM, 0, 1023, 1, 700);
        delayValue = 100;

        for(i = 0; i < count; i++)
            {
                delayValue = delayValue + (0.01 * delayValue);
            }
*/
        if(!(P2IN & SW))            // If SW is Pressed
        {
            __delay_cycles(20000);  // Wait 20ms to debounce
            while(!(P2IN & SW));    // Wait till SW Released
            __delay_cycles(20000);  // Wait 20ms to debounce
            displayValue = displayValue + 1;
            if(displayValue > 9999)
                displayValue = 0;
        }

        fourDigitNumber(avgADC);
        P2OUT &=~ (SEG_1 + SEG_2 + SEG_3 + SEG_4);
    }
}
