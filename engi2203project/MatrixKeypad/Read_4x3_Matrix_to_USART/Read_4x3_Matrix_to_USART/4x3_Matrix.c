/*
 * 4x3_Matrix.c
 *
 * Created: 2/19/2020 7:24:54 PM
 *  Author: NickCestnick
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include "4x3_Matrix.h"
 
int getKeyPress(void)
{
	PORTB |= (1<<PORTB0) | (1<<PORTB1) | (1<<PORTB2) | (1<<PORTB3); //Set Row Ports High
	PORTD &= ~(1<<PORTD5) & ~(1<<PORTD6) & ~(1<<PORTD7); //Sets Column Ports Low
	int c;
	int r;
	for(c = 0; c < 3; c++) //Loops Once per column
	{
		DDRB &= ~(0xFF); // Sets all Pins to Input
		DDRD &= ~(0xE0); //  "
		DDRD |= (0x80>>c); //sets one column to write per loop
		_delay_ms(10);
		for(r = 0; r < 4; r++) // Loops once per row
		{
			if(!(PINB & (0x08>>r))) // Senses if button in current row is pressed
			{
				int output = (r*3+c);
				return  output; //returns the value of the pressed button
			}
		}
		
	}
	return 0xFF; // returns value to indicate no button pressed
}
