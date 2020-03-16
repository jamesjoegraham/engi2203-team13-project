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
#define hallSensor PORTB0
#define LED PORTB1


int detect(void) {
	DDRB &= ~(1<<hallSensor);        //set the data direction to input
	DDRB |= (1<<LED);                //make led output

	while(1) {
		if(!(PINB & (1<<hallSensor))){
			PORTB &= ~(1<<LED);
		}
		else {
			PORTB |= (1<<LED);
		}
	}
}





