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
   init_uart();
   
   int keypress;

   while (1)
   {
	   keypress = getKeyPress()+1;
	   if(keypress < 10){
		   printf("%d\n", keypress);
		   _delay_ms(250);
	   }
	   else if(keypress == 10){
		   printf("*\n");
		   _delay_ms(250);
	   }
	   else if(keypress == 11){
		   printf("0\n");
		   _delay_ms(250);
	   }
	   else if(keypress==12){
		   printf("#\n");
		   _delay_ms(250);
	   }else
	   {
	   }
   }
   return 0;
}

