#include <msp430.h> 
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <Library/LunchboxCommon.h>
#include <Examples/Example_common.h>

#define demo_data_types_code    1
#define demo_bit_manipulation_code  2

#define Target_code demo_bit_manipulation_code
/*
 * An example of multi-line comments
 * @brief entry point for the code
 * */
int main(void)
{ //! Stop Watchdog (Not recommended for code in production and devices working in field)
WDTCTL = WDTPW | WDTHOLD;

initialise_SerialPrint_on_lunchbox(); // a function
while (1)
{
#if (Target_code == demo_data_types_code)
    demo_data_types();
#elif(Target_code == demo_bit_manipulation_code)
    demo_bit_manipulation();
#endif

    unsigned long delay;
    for (delay = 0; delay < 60000; delay++);
}
}

