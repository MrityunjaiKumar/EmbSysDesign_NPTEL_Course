#include <msp430.h>
#include <inttypes.h>
#include <stdio.h>

#define CMD         0
#define DATA        1

#define LCD_OUT     P1OUT
#define LCD_DIR     P1DIR
#define D4          BIT4
#define D5          BIT5
#define D6          BIT6
#define D7          BIT7
#define RS          BIT2
#define EN          BIT3

volatile unsigned int count = 0, edge1, edge2, period;  // Global variables
volatile unsigned long freq;
char freqDisplay[15];
volatile unsigned int i;


/**
 *@brief Delay function for producing delay in 0.1 ms increments
 *@param t milliseconds to be delayed
 *@return void
 **/
void delay(uint16_t t)
{
    uint16_t i;
    for(i=t; i > 0; i--)
        __delay_cycles(100);
}

/**
 *@brief Function to pulse EN pin after data is written
 *@return void
 **/
void pulseEN(void)
{
    LCD_OUT |= EN;
    delay(1);
    LCD_OUT &= ~EN;
    delay(1);
}

/**
 *@brief Function to write data/command to LCD
 *@param value Value to be written to LED
 *@param mode Mode -> Command or Data
 *@return void
 **/
void lcd_write(uint8_t value, uint8_t mode)
{
    if(mode == CMD)
        LCD_OUT &= ~RS;             // Set RS -> LOW for Command mode
    else
        LCD_OUT |= RS;              // Set RS -> HIGH for Data mode

    LCD_OUT = ((LCD_OUT & 0x0F) | (value & 0xF0));              // Write high nibble first
    pulseEN();
    delay(1);

    LCD_OUT = ((LCD_OUT & 0x0F) | ((value << 4) & 0xF0));       // Write low nibble next
    pulseEN();
    delay(1);
}

/**
 *@brief Function to print a string on LCD
 *@param *s pointer to the character to be written.
 *@return void
 **/
void lcd_print(char *s)
{
    while(*s)
    {
        lcd_write(*s, DATA);
        s++;
    }
}

void lcd_printNumber(unsigned int num)
{
    char buf[6];
    char *str = &buf[5];

    *str = '\0';

    do
    {
        unsigned long m = num;
        num /= 10;
        char c = (m - 10 * num) + '0';
        *--str = c;
    } while(num);

    lcd_print(str);
}

/**
 *@brief Function to move cursor to desired position on LCD
 *@param row Row Cursor of the LCD
 *@param col Column Cursor of the LCD
 *@return void
 **/
void lcd_setCursor(uint8_t row, uint8_t col)
{
    const uint8_t row_offsets[] = { 0x00, 0x40};
    lcd_write(0x80 | (col + row_offsets[row]), CMD);
    delay(1);
}

/**
 *@brief Initialize LCD
 **/
void lcd_init()
{
    LCD_DIR |= (D4+D5+D6+D7+RS+EN);
    LCD_OUT &= ~(D4+D5+D6+D7+RS+EN);

    delay(150);                     // Wait for power up ( 15ms )
    lcd_write(0x33, CMD);           // Initialization Sequence 1
    delay(50);                      // Wait ( 4.1 ms )
    lcd_write(0x32, CMD);           // Initialization Sequence 2
    delay(1);                       // Wait ( 100 us )

    // All subsequent commands take 40 us to execute, except clear & cursor return (1.64 ms)

    lcd_write(0x28, CMD);           // 4 bit mode, 2 line
    delay(1);

    lcd_write(0x0C, CMD);           // Display ON, Cursor OFF, Blink OFF
    delay(1);

    lcd_write(0x01, CMD);           // Clear screen
    delay(20);

    lcd_write(0x06, CMD);           // Auto Increment Cursor
    delay(1);

    lcd_setCursor(0,0);             // Goto Row 1 Column 1
}

/**
 *@brief Displays on LCD
 **/
void lcd_display()
{
    lcd_write(0x01, CMD);                       // Clear screen
    delay(20);
    lcd_setCursor(0,1);
    lcd_print("Hello Embedded");
    lcd_setCursor(1,0);
    lcd_printNumber(edge1);
    lcd_print("Hz");
    delay(10000);
}

/**
 * @brief
 * These settings are w.r.t enabling TIMER1 on Lunch Box
 **/
void register_settings_for_TIMER1()
{
    //register_settings_for_TIMER0();                 //Initialize Timer1

    TA1CCTL0 = CCIE;                       // CCR0 interrupt enabled
    TA1CCR0 =  32768;                        // 1 Hz
    TA1CTL = TASSEL_1 + MC_1 + TACLR;              // ACLK = 32768 Hz, upmode
}

/*@brief entry point for the code*/
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                       //! Stop Watchdog (Not recommended for code in production and devices working in field)

    unsigned int i;

    do{
             IFG1 &= ~OFIFG;                     // Clear oscillator fault flag
             for (i = 50000; i; i--);            // Delay
        } while (IFG1 & OFIFG);               // Test osc fault flag

    P1DIR &=~ BIT0;
    P1SEL |= BIT0;

    BCSCTL2 |= DIVS_2;
    lcd_init();                                     // Initialize LCD
    register_settings_for_TIMER1();
    register_settings_for_TIMER0_enable();
    __bis_SR_register(GIE);

    while(1)
    {
       lcd_display();
    }

}

*/
/*@brief entry point for TIMER0 interrupt vector*/
#pragma vector= TIMER1_A0_VECTOR
__interrupt void Timer_A (void)
{
    if(count == 0)
    {
        count++;

        TA0CTL |= TASSEL_2 + MC_2 + TACLR;                      // Clock -> TACLK, Cont. Mode, Clear Timer
    }
    else
    {
        TA0CTL = MC_0;                                          // Stop
        edge1 = TAR;
        count = !count;
    }
}
