#include <msp430.h>
#include <stdio.h>
#include <string.h>

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


#include <stdint.h>
uint16_t lfsr = 0xACE1u;
unsigned period = 0;


/*@brief entry point for the code*/
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;           //! Stop Watchdog (Not recommended for code in production and devices working in field)

    unsigned int i = 0;

    register_settings_for_UART();
    for (i = 0; i < 60000; i++);
    for (i = 0; i < 60000; i++);

    while (1)
    {

 //       do {
 //       unsigned lsb = lfsr & 1;  /* Get lsb (i.e., the output bit). */
 //       lfsr >>= 1;               /* Shift register */
 //       if (lsb == 1)             /* Only apply toggle mask if output bit is 1. */
 //       lfsr ^= 0xB400u;        /* Apply toggle mask, value has 1 at bits corresponding
 //                                * to taps, 0 elsewhere. */
 //
 //       printf("Hello world :  %d!\r\n", lfsr);
 //       for (i = 0; i < 60000; i++);
 //       } while(lfsr != 0xACE1u);
        unsigned lsb = ((lfsr >> 31) ^ (lfsr >> 29) ^ (lfsr >> 28) ^ (lfsr >> 26));
        lfsr = ((lfsr >> 1) | (lsb << 31));
        printf("Hello world :  %d!\r\n", lfsr);
        for (i = 0; i < 60000; i++);
    }
}
