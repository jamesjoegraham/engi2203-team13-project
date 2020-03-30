/*
 * uart_utility_functions.h
 *
 * Created: 2020-03-21 3:24:20 PM
 *  Author: james
 */ 


#ifndef UART_UTILITY_FUNCTIONS_H_
#define UART_UTILITY_FUNCTIONS_H_

/*
 * GccApplication7.c
 *
 * Created: 2017-01-03 10:27:53 AM
 * Author : JF
 */ 


#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

int uart_putchar(char c, FILE *stream);

int uart_getchar(FILE *stream);

int uart_putchar(char c, FILE *stream);

int uart_getchar(FILE *stream);

void init_uart(void);



#endif /* UART_UTILITY_FUNCTIONS_H_ */