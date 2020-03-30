/*
 * Lab4UltraSonicSensor.c
 *
 * Created: 2020-03-03 1:54:43 PM
 * Author : sahil
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdio.h>

#include "ultrasound.h"

#include "../uart_utility_functions.h"

volatile unsigned char MIP;
volatile unsigned int ECHOHigh, ECHOLow, ECHOHighPipe;
volatile unsigned int TimeOutCnt,Tick;

/***************************************************************************************
When the echo length is longer than the counter duration, we use an additional byte to
indicate how many times we reach the maximum value.
***************************************************************************************/
ISR (TIMER1_OVF_vect) {	// For long ECHO's
	if(ECHOHighPipe >= 2) {
		TIMSK1 = 0;	// No further interrupts.
		TCCR1B = 0; // Stop Clock
		MIP = 0;	// End Measurement
	}
	
	printf("ovf\n");
	ECHOHighPipe++;	// Add 1 to High byte.
}

/***************************************************************************************
Interrupt service routine called when the input capture pin state is changed
***************************************************************************************/
ISR (TIMER1_CAPT_vect) {	// Start and Stop ECHO measurement;
	printf("Echo meas\n");
	if((TCCR1B & (1 << ICES1)) != 0) { // a rising edge has been detected
		TCCR1B |= (1<<CS11);	// Start counting with ck/8;
		TCCR1B &= ~(1 << ICES1);  // Configure Negative Edge Capture for end of echo pulse.
	}
	
	else {						// a falling edge has been detected
		ECHOLow = TCNT1;
		ECHOHigh = ECHOHighPipe;
		TIMSK1 = (1 << OCIE1B);	// Enables the Compare B interrupt for POST Trigger Delay: Approx 10mS
		TCNT1 = 0;
	}
}


/***************************************************************************************
Interrupt service routine called when the counter 1 has reached the compare value
***************************************************************************************/

ISR (TIMER1_COMPB_vect)
{
	MIP = 0;
}

/***************************************************************************************
Interrupt service routine called when the counter 1 has reached the compare value
***************************************************************************************/
/*
ISR (TIMER1_COMPA_vect) {	// Compare A : End of Trigger Pulse
	USPORT &= ~(1 << USPORTOUT);
	TIMSK1 = (1<<ICIE1)|(1<<TOIE1); // enables the T/C1 Overflow and Capture interrupt;
	TCCR1B = (1<<ICES1);			// Set Positive edge for capture but Don't count yet
}*/

/******************************************************************************
******************************************************************************/
void Trigger(void) {		// Config Timer 1 for 10 to 15uS pulse.
	if(MIP == 0) {	// Don't allow re-trigger.
		cli();
		MIP = 1;				// Set Measurement in progress FLAG
		USPORTDD |= (1 << USPORTOUT);		// PB1 as Output for Trigger pulse.
		USPORTDD &= ~(1 << USPORTIN);		// PB0 as Input for Input Capture (ECHO).
		
		TCNT1 = 0;				// Clear last Echo times.
		ECHOHighPipe = 0;
		
		OCR1B = 10100;			// 10 mS Post echo Delay
		OCR1A = 12;				// 10 us Trigger length.


		USPORT |= (1 << USPORTOUT);		// Start Pulse.

		TIFR1 = 0xFF;			//  Clear all timer interrupt flags
		TCCR1A = 0;   // Timer mode with Clear Output on Match
		TCCR1B = (1<<WGM12) | (1<<CS11);  // Counting with CKio/8 CTC Mode enabled
		TIMSK1 = (1<<OCIE1A);	// enables the T/C1 Overflow, Compare A, and Capture interrupt;
		sei();
	}
	
}

/******************************************************************************
******************************************************************************/
int ultraDistance(void) {
	//sei();
	int tim, dist;
	int detect = 58;
	
	printf("Measuring Ultrasonic Ranger\n");

	Trigger();	
	while (MIP == 1){};
	tim = (65536*ECHOHigh + ECHOLow) / 2;
	dist = tim /58;
	printf("Distance is %d\n", dist);

	if (dist < detect) return 1;
	else return 0;
}
