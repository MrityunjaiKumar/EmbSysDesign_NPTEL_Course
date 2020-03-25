#include <msp430.h>
#include <inttypes.h>

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

#define SW          BIT3

#define LCD_SETCGRAMADDR 0x40

//Heart character
uint8_t heart[8] = {
  0x00,
  0x0A,
  0x1F,
  0x1F,
  0x1F,
  0x0E,
  0x04,
  0x00
};

//
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
    LCD_OUT |= EN;                              // Giving a falling edge at EN pin
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
 *@brief Allows us to fill the first 8 CGRAM locations with custom characters
 *@param location Row Cursor of the LCD
 *@param charmap Column Cursor of the LCD
 *@return void
 **/
void lcd_createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  lcd_write(LCD_SETCGRAMADDR | (location << 3), CMD);
  int i = 0;
  for (i=0; i<8; i++) {
      lcd_write(charmap[i], DATA);
  }
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

    lcd_write(0x06, CMD);           // Auto Increment Cursor
    delay(1);

    lcd_write(0x01, CMD);           // Clear screen
    delay(20);

    lcd_setCursor(0,0);             // Goto Row 1 Column 1
}

/*@brief entry point for the code*/
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;       //! Stop Watchdog (Not recommended for code in production and devices working in field)

    uint8_t count = 0;

    P2DIR &=~ SW;

    lcd_init();                             // Initialising LCD

    lcd_createChar(0, heart);                // Creating Custom Character

    lcd_setCursor(0,1);                     // Cursor position (0,1)
    lcd_print("Hello Embedded");            // Print

    lcd_setCursor(1,4);                     // Cursor position (1,3)
    lcd_print("Systems");                   // Print

    while(1)
    {
        if(!(P2IN & SW))            // If SW is Pressed
        {
            __delay_cycles(20000);  // Wait 20ms to debounce
            while(!(P2IN & SW));    // Wait till SW Released
            __delay_cycles(20000);  // Wait 20ms to debounce
            switch(count)
            {
                case 0:
                {
                    lcd_setCursor(1,3);                     // Cursor position (1,3)
                    lcd_write(0x00, DATA);                  // Printing Custom Char (Heart)
                    lcd_setCursor(1,11);                    // Cursor position (1,11)
                    lcd_write(0x00, DATA);                  // Printing Custom Char (Heart)
                    count = 1;
                    break;
                }

                case 1:
                {
                    lcd_setCursor(1,3);                     // Cursor position (1,3)
                    lcd_write(0x20, DATA);                  // Printing Space
                    lcd_setCursor(1,11);                    // Cursor position (1,11)
                    lcd_write(0x20, DATA);                  // Printing Space
                    count = 0;
                    break;
                }
            }
        }
    }
}
