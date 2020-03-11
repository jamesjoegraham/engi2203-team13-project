/*
 * Keypad Funtion as Header.c
 *
 * Created: 2/19/2020 7:06:26 PM
 * Author : WDTV
 */ 

#include <avr/io.h>
#include <stdio.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "4x3_Matrix.h"

int main(void)
{
	initHardware();
	init_uart();
   
	char keypress;

	while (1)
	{
		keypress = getNewKeypress();
		
		if(keypress)printf("%c\n", keypress);
		
		
	   
	}
	return 0;
}

