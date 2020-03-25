#include <msp430.h> 

/*@brief entry point for the code*/
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   //! Stop Watchdog (Not recommended for code in production and devices working in field)

    while(1){

    }
}
