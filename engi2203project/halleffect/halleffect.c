/*
 * HallEffectSensor.c
 *
 * Created: 2020-03-04 7:59:57 PM
 * Author : sahil
 */ 

/*
PBO - INPUT
PB1 - OUTPUT

DDRB TO TURN SOMETHING INTO INPUT OR OUTPUT

SET THE DIRECTION OF PINB RESISTOR TO MAKE IT INPUT
AND CHANGE THE DIRECTION OF PINB RESISTOR TO MAKE IT OUTPUT
to make a pin input send a value of 0 to DDRB resistor
when switch is released its 0 */

#include <avr/io.h>

#include "halleffect.h"


int detectHallEffect()
{
	hallDD &= ~(1 << hallSensor); // turn DD off for input
	//	DDRB &= ~(1 << PORTB5);
	
	if (hallPort & (1 << hallSensor)) // if HIGH
	{
		return 1;
	}
	else
	{
		return 0;
	}
}





