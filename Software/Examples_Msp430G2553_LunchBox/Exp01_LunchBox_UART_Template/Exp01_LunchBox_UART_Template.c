#include <Library/LunchboxCommon.h>
#include <msp430.h> 
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

/*
 * An example of multi-line comments
 * @brief entry point for the code
 */
int main(void)
{
    //! Stop Watchdog (Not recommended for code in production and devices working in field)
    WDTCTL = WDTPW | WDTHOLD;

    initialise_SerialPrint_on_lunchbox(); // a function
    int x = 0;
    while (1)
    {
        x++;
        printf("Hello world %d \r\n", x);

        int i;
        for (i = 0; i < 20000; i++);
    }
}
