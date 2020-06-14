#ifndef _LUNCHBOX_COMMON_H
#define _LUNCHBOX_COMMON_H
/**
 * @brief
 * preprocessor directive for uart
 * For overriding the fputc() and fputs() functions
 **/
#define UART_PRINTF

void initialise_uart_on_lunchbox();

#endif // !_LUNCHBOX_COMMON_H