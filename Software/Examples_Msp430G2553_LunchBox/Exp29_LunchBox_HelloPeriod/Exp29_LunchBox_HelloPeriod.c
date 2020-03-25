#include <msp430.h>
#include <inttypes.h>
#include <stdio.h>

#define CMD         0
#define DATA       1

#define LCD_OUT     P1OUT
#define LCD_DIR     P1DIR
#define D4          BIT4
#define D5          BIT5
#define D6          BIT6
#define D7          BIT7
#define RS          BIT1
#define EN          BIT3

volatile unsigned char count;
volatile unsigned int edge1, edge2, period;  // Global variables
volatile double freq, time;


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

/**
 *@brief Function to convert number into character array
 *@param num integer number to be converted.
 *@return void
 **/
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
    int int_part_time = time;                                   // Integer part of captured time
    int decimal_part_time = (time - (float)int_part_time) * 100.0 ;   // Decimal part of captured time

    int int_part_freq = freq;                                   // Integer part of captured frequency
    int decimal_part_freq = (freq - (float)int_part_freq) * 100.0 ;   // Decimal part of captured frequency

    lcd_write(0x01, CMD);                       // Clear screen
    delay(20);
    lcd_setCursor(0,1);
    lcd_print("Period : ");
    lcd_printNumber(int_part_time);
    lcd_print(".");
    lcd_printNumber(decimal_part_time);
    lcd_print("s");
    lcd_setCursor(1,1);
    lcd_print("Freq   : ");
    lcd_printNumber(int_part_freq);
    lcd_print(".");
    lcd_printNumber(decimal_part_freq);
    lcd_print("Hz");
    delay(10000);
}

/**
 * @brief
 * These settings are wrt enabling TIMER0 on Lunchbox
 **/
void register_settings_for_TIMER0()
{
    P1DIR &= ~BIT2;                                 // Set P1.2 -> Input
    P1SEL |= BIT2;                                  // Set P1.2 -> TA0.1 Capture Mode

    TA0CCTL1 = CAP + CM_1 + CCIE + SCS + CCIS_0;    // Capture Mode, Rising Edge, Interrupt
                                                    // Enable, Synchronize, Source -> CCI0A
    TA0CTL |= TASSEL_1 + MC_2 + TACLR + ID_2;       // Clock -> ACLK/4, Cont. Mode, Clear Timer
}

/*@brief entry point for the code*/
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                       //! Stop Watchdog (Not recommended for code in production and devices working in field)

    lcd_init();                                     // Initialize LCD

    register_settings_for_TIMER0();                 //Initialize Timer0

    while(1)
    {
        lcd_display();
        __bis_SR_register(LPM0_bits + GIE);         // Enter LPM0, Enable Interrupt

        //Exits LPM0 after 2 rising edges are captured
        if(edge2 > edge1)
        {
            period =  edge2 - edge1;                 // Calculate Period
            freq = (32768.0/period) / 4;
            time = (period /32768.0) * 4;
        }
     }
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR (void)
{
    switch(__even_in_range(TA0IV,0x0A))
    {
        case  TA0IV_NONE: break;                            // Vector  0:  No interrupt

        case  TA0IV_TACCR1:                                 // Vector  2:  TACCR1 CCIFG

            if (count == 0)                                     // Check value of count
            {
                edge1 = TA0CCR1;                            // Store timer value of 1st edge
                count++;                                    // Increment count
            }
            else
            {
                edge2 = TA0CCR1;                            // Store timer value of 2nd edge
                count=0;                                    // Reset count
                __bic_SR_register_on_exit(LPM0_bits + GIE); // Exit LPM0 on return to main
            }
            break;

        case TA0IV_TACCR2: break;                           // Vector  4:  TACCR2 CCIFG
        case TA0IV_6: break;                                // Vector  6:  Reserved CCIFG
        case TA0IV_8: break;                                // Vector  8:  Reserved CCIFG
        case TA0IV_TAIFG: break;                            // Vector 10:  TAIFG
        default:    break;
    }
}
