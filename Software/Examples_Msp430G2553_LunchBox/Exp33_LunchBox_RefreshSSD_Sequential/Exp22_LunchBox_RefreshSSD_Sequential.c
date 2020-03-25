#include <msp430.h>
#include <inttypes.h>

#define AIN     BIT0                    // Potentiometer -> P1.0

#define SW      BIT3                    // Switch -> P1.3

// Define Pin Mapping of 7-segment Display
// Segment A is connected to P2.5, Segment D is connected to P2.4
// Segments B,C,E,F,G and DP are connected to P1.1, P1.2, P1.4, P1.5, P1.6, P1.7 respectively
#define SEG_A   BIT5
#define SEG_B   BIT1
#define SEG_C   BIT2
#define SEG_D   BIT4
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
        __delay_cycles(1500);
}

/**
 *@brief This function displays digit on single Seven Segment Display
 *@param digit Digit to be displayed
 *@return void
 **/
void digitToDisplay(unsigned int numberToBeDisplayed){

    switch(numberToBeDisplayed){
        case 0: P2OUT |= (SEG_A + SEG_D);
                P1OUT |= SEG_B + SEG_C + SEG_E + SEG_F;
                break;

        case 1: P1OUT |= SEG_B + SEG_C;
                break;

        case 2: P2OUT |= (SEG_A + SEG_D);
                P1OUT |= SEG_B + SEG_E + SEG_G;
                break;

        case 3: P2OUT |= (SEG_A + SEG_D);
                P1OUT |= SEG_B + SEG_C + SEG_G;
                break;

        case 4: P1OUT |= SEG_B + SEG_C + SEG_F + SEG_G;
                break;

        case 5: P2OUT |= (SEG_A + SEG_D);
                P1OUT |= SEG_C + SEG_F + SEG_G;
                break;

        case 6: P2OUT |= (SEG_A + SEG_D);
                P1OUT |= SEG_C + SEG_E + SEG_F + SEG_G;
                break;

        case 7: P2OUT |= SEG_A;
                P1OUT |= SEG_B + SEG_C;
                break;

        case 8: P2OUT |= (SEG_A + SEG_D);
                P1OUT |= SEG_B + SEG_C + SEG_E + SEG_F + SEG_G;
                break;

        case 9: P2OUT |= (SEG_A + SEG_D);
                P1OUT |= SEG_B + SEG_C  + SEG_F + SEG_G;
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


  P1OUT &=~ (SEG_B + SEG_C + SEG_E + SEG_F + SEG_G);
  P2OUT &=~ (SEG_A + SEG_D);
  P2OUT &=~ (SEG_1 + SEG_2 + SEG_3 + SEG_4);

  digitToDisplay(displayDigit[0]);    // Display first digit
  P2OUT |= SEG_4;
  delay(delayValue);

  P1OUT &=~ (SEG_B + SEG_C + SEG_E + SEG_F + SEG_G);
  P2OUT &=~ (SEG_A + SEG_D);
  P2OUT &=~ (SEG_1 + SEG_2 + SEG_3 + SEG_4);

  digitToDisplay(displayDigit[1]);    // Display second digit
  P2OUT |= SEG_3;
  delay(delayValue);

  P1OUT &=~ (SEG_B + SEG_C + SEG_E + SEG_F + SEG_G);
  P2OUT &=~ (SEG_A + SEG_D);
  P2OUT &=~ (SEG_1 + SEG_2 + SEG_3 + SEG_4);

  digitToDisplay(displayDigit[2]);    // Display third digit
  P2OUT |= SEG_2;
  delay(delayValue);

  P1OUT &=~ (SEG_B + SEG_C + SEG_E + SEG_F + SEG_G);
  P2OUT &=~ (SEG_A + SEG_D);
  P2OUT &=~ (SEG_1 + SEG_2 + SEG_3 + SEG_4);

  digitToDisplay(displayDigit[3]);    // Display forth digit
  P2OUT |= SEG_1;
  delay(delayValue);
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

/**
 * @brief
 * These settings are w.r.t GPIO on Lunch Box
 **/
void initialize_GPIO()
{
    // Setting Pins connected to SSD as OUTPUT
    P1DIR |= (SEG_B + SEG_C  + SEG_E + SEG_F + SEG_G + SEG_DP);
    P2DIR |= (SEG_A + SEG_D);
    P2DIR |= (SEG_1 + SEG_2 + SEG_3 + SEG_4);

    // Setting Pins connected to SSD as LOW
    P1OUT &=~ (SEG_B + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G);
    P2OUT &=~ (SEG_A + SEG_D);
    P2OUT &=~ (SEG_1 + SEG_2 + SEG_3 + SEG_4);

    // Setting Switch pin as input
    P1DIR &=~ (SW);
}


/*@brief entry point for the code*/
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;                   //! Stop Watchdog (Not recommended for code in production and devices working in field)

    BCSCTL1 |= (BIT0 + BIT1 + BIT2 + BIT3);     // Selecting RSELx as 15
    DCOCTL  |= (BIT6 + BIT5 + BIT4);            // Selecting DCOx as 7, DCO_freq = 15.6 MHz

    initialize_GPIO();                          // GPIO setting

    register_settings_for_ADC10();              // Setting for ADC10

    while(1)
    {
        ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
        while(ADC10CTL1 & ADC10BUSY);           // Wait for conversion to end
        int adcValue1 = ADC10MEM;               // ADC value 1

        ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
        while(ADC10CTL1 & ADC10BUSY);           // Wait for conversion to end
        int adcValue2 = ADC10MEM;               // ADC value 2

        int avgADC = (adcValue1 + adcValue2) / 2;   // Average  of two values

        if (avgADC <500)                            // Calculating delay value
            delayValue = (1 + .1*avgADC) * 10.0;
        else
            delayValue = (51 + 0.8*(avgADC - 500)) * 10.0;


        if(!(P1IN & SW))                        // If SW is Pressed
        {
            __delay_cycles(200);                // Wait 20ms to debounce
            while(!(P1IN & SW));                // Wait till SW Released
            __delay_cycles(200);                // Wait 20ms to debounce
            displayValue = displayValue + 1;
            if(displayValue > 9999)
                displayValue = 0;
        }

        fourDigitNumber(displayValue);
        P2OUT &=~ (SEG_1 + SEG_2 + SEG_3 + SEG_4);
    }
}
