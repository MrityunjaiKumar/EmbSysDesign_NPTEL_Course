#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define AIN0     BIT0
#define AIN1     BIT1
#define AIN2     BIT2
#define AIN3     BIT3

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
 * preprocessor directive for uart
 * For overriding the fputc() and fputs() functions
 **/
#define UART_PRINTF

#ifdef UART_PRINTF
/**
 *@brief This function overrides fputc()
 *@param *_ptr pointer to the character to be written.
 *@param *_fp  pointer to a FILE object that identifies the stream where the character is to be written.
 *@return length of the character stream
 **/
int fputc(int _c, register FILE *_fp)
{
    while (!(IFG2 & UCA0TXIFG))
        ;
    UCA0TXBUF = (unsigned char) _c;

    return ((unsigned char) _c);
}

/**
 *@brief This function overrides fputs
 *@param *_ptr pointer to the character to be written.
 *@param *_fp  pointer to a FILE object that identifies the stream where the character is to be written.
 *@return length of the character stream
 **/
int fputs(const char *_ptr, register FILE *_fp)
{
    unsigned int i, len;

    len = strlen(_ptr);

    for (i = 0; i < len; i++)
    {
        while (!(IFG2 & UCA0TXIFG))
            ;
        UCA0TXBUF = (unsigned char) _ptr[i];
    }

    return len;
}

#endif

/**
 * @brief
 * These settings are wrt enabling ADC10 on Lunchbox
 **/
void register_settings_for_ADC10()
{
    ADC10AE0 |= AIN0 + AIN1 + AIN2 + AIN3;                            // P1.0 ADC option select
    ADC10CTL1 = INCH_0 + INCH_1 + INCH_2 + INCH_3;                         // ADC Channel -> 1 (P1.0)
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON;  // Ref -> Vcc, 64 CLK S&H
}

/**
 * @brief
 * These settings are wrt enabling uart on Lunchbox
 **/
void register_settings_for_UART()
{
    P1SEL = BIT1 + BIT2;              // Select UART RX/TX function on P1.1,P1.2
    P1SEL2 = BIT1 + BIT2;

    UCA0CTL1 |= UCSSEL_1;               // UART Clock -> ACLK
    UCA0BR0 = 3;                        // Baud Rate Setting for 1MHz 9600
    UCA0BR1 = 0;                        // Baud Rate Setting for 1MHz 9600
    UCA0MCTL = UCBRF_0 + UCBRS_3;       // Modulation Setting for 1MHz 9600
    UCA0MCTL &= ~UCOS16;
    UCA0CTL1 &= ~UCSWRST;               // Initialize UART Module
    IE2 |= UCA0RXIE;                    // Enable RX interrupt
}

/*@brief entry point for the code*/
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;           //! Stop Watchdog (Not recommended for code in production and devices working in field)

    unsigned int counter = 0;

    register_settings_for_ADC10();
    register_settings_for_UART();

    unsigned int i = 0;
    while (1)
    {
        ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start

        while(ADC10CTL1 & ADC10BUSY);           // Wait for conversion to end

        printf("Hello world %d!\r\n", rand());
        for (i = 0; i < 60000; i++);
    }
}
