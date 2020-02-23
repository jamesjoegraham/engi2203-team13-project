/*
 * engi2203project.c
 *
 * Created: 2020-02-11 2:46:10 PM
 * Author : james
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "MatrixKeypad/4x3_Matrix.h"
#include "sound/sound.h"
#include "alarm/alarm.h"

// KEYPAD CODE
char* keypass = "1234";


unsigned long int c;
// interrupt
ISR (TIMER1_OVF_vect)
{
	c++;
}


int main(void)
{
	// program start	
	// initialize stuff
	
	c = 0;
	TIMSK1 = (1 << TOIE1);
	TCCR1B = (1 << CS12) || (1 << CS10);
	TCNT1 = 65520;
	sei();
	
	DDRB = 0xFF;
	DDRC = 0xFF;
	
	// main loop
    while (1) 
    {
		// get key press
		//int key = getKeyPress();
		// if waiting too long, clear key press history
		
		
		// call to PIR sensor, if on enable alarm
		
		
		if (c < 3000) disarm();
		else if (c < 6000) arm();
		else if (c < 9000) pdetect();
		else
		{
			alarm(c);
		}
		
		// call to ultrasound sensor, if on commence countdown
		
		//beepTone(5000);
		
		
    }
}

