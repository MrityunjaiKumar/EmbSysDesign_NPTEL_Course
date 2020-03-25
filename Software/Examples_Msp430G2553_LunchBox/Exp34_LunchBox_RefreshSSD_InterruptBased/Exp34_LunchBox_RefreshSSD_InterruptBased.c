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

volatile unsigned int displayValue = 0;                         // Variable to be displayed
volatile unsigned char displayDigit[4];                         // Array to store individual digit of DisplayValue
volatile unsigned char digit = 4;                               // Variable to store digit to be displayed
volatile int adcValue;                                          // Variable to capture ADC value

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
 *@brief This function separates 4 digits from a number.
 *@param number Number to be displayed
 *@return Void
 **/
void fourDigitNumber(int number)
{
  displayDigit[0] = number / 1000;
  number = number % 1000;
  displayDigit[1] = number / 100;
  number = number % 100;
  displayDigit[2] = number / 10;
  displayDigit[3] = number % 10;
}

/**
 *@brief This function displays the number on a four digit seven segment display
 *@param number Number to be displayed
 *@return Void
 **/
void displayNumber()
{
  P1OUT &=~ (SEG_B + SEG_C + SEG_E + SEG_F + SEG_G);
  P2OUT &=~ (SEG_A + SEG_D);
  P2OUT &=~ (SEG_1 + SEG_2 + SEG_3 + SEG_4);

  switch(digit)
  {
      case 4:
          digitToDisplay(displayDigit[0]);    // Display first digit
          P2OUT |= SEG_4;
          break;
      case 3:
          digitToDisplay(displayDigit[1]);    // Display second digit
          P2OUT |= SEG_3;
          break;
      case 2:
          digitToDisplay(displayDigit[2]);    // Display third digit
          P2OUT |= SEG_2;
          break;
      case 1:
          digitToDisplay(displayDigit[3]);    // Display forth digit
          P2OUT |= SEG_1;
          break;
  }
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
    ADC10CTL0 = ADC10IE;                        // Set ADC module
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON;  // Ref -> Vcc, 64 CLK S&H, ADC10 enable
}

/**
 * @brief
 * These settings are w.r.t enabling TIMER0 on Lunch Box
 **/
void register_settings_for_TIMER0()
{
    CCTL0 = CCIE;                               // CCR0 interrupt enabled
    TACTL = TASSEL_1 + MC_1;                    // ACLK = 32768/4 Hz, up to CCR0
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

    BCSCTL1 = DIVA_2;                           // ACLK divider : /4

    initialize_GPIO();                          // GPIO setting

    register_settings_for_ADC10();              // Setting for ADC10

    register_settings_for_TIMER0();             // Setting for TIMER0

    __bis_SR_register(GIE);                     // Enable CPU Interrupt
    while(1)
    {
        if(!(P1IN & SW))                        // If SW is Pressed
        {
            __delay_cycles(200);                // Wait 20ms to debounce
            while(!(P1IN & SW));                // Wait till SW Released
            __delay_cycles(200);                // Wait 20ms to debounce
            displayValue = displayValue + 1;
            if(displayValue > 9999)
                displayValue = 0;
        }

        ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start

        while(ADC10CTL1 & ADC10BUSY);           // Wait for conversion to end

        int adcValue = ADC10MEM;                // Saving ADC value to adcValue

        if (adcValue <500)                      // Calculating TIMER0 compare register value
            CCR0 = (1 + .1 * adcValue) * 10.0;
        else
            CCR0 = (51 + 0.8 * (adcValue - 500)) * 10.0;

        fourDigitNumber(displayValue);          // Diving number to be displayed into 4 separate digits.
    }
}

/*@brief entry point for TIMER0 interrupt vector*/
#pragma vector= TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    digit--;
    if(digit == 0)
        digit = 4;
    displayNumber();                            // Display number according to the digit value
}
