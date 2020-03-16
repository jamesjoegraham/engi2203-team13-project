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
#include <math.h>


volatile unsigned char MIP;
volatile unsigned int ECHOHigh, ECHOLow, ECHOHighPipe;
volatile unsigned int TimeOutCnt,Tick;
void init_uart(void);

int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);
FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE mystdin = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);
/******************************************************************************
******************************************************************************/
int uart_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}
/******************************************************************************
******************************************************************************/
int uart_getchar(FILE *stream)
{
	/* Wait until data exists. */
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}
/******************************************************************************
******************************************************************************/
void init_uart(void)
{
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	UBRR0 = 103;  //With 16 MHz Crystal, 9600 baud = 103
	stdout = &mystdout;
	stdin = &mystdin;
}
/***************************************************************************************
When the echo length is longer than the counter duration, we use an additional byte to
indicate how many times we reach the maximum value.
***************************************************************************************/
ISR (TIMER1_OVF_vect) {	// For long ECHO's
	if(ECHOHighPipe >= 2) {
		TIMSK1 = 0;	// No further interrupts.
		TCCR1B = 0; // Stop Clock
		MIP = 0xFF;	// End Measurement
	}
	
	ECHOHighPipe++;	// Add 1 to High byte.
}

/***************************************************************************************
Interrupt service routine called when the input capture pin state is changed
***************************************************************************************/
ISR (TIMER1_CAPT_vect) {	// Start and Stop ECHO measurement;
	if((TCCR1B & (1<<ICES1)) != 0) { // a rising edge has been detected
		TCCR1B |= (1<<CS11);	// Start counting with ck/8;
		TCCR1B &= ~(1<<ICES1);  // Configure Negative Edge Capture for end of echo pulse.
	}
	
	else {						// a falling edge has been detected
		ECHOLow = TCNT1;
		ECHOHigh = ECHOHighPipe;
		TIMSK1 = (1<<OCIE1B);	// Enables the Compare B interrupt for POST Trigger Delay: Approx 10mS
		TCNT1 = 0;
	}
}


/***************************************************************************************
Interrupt service routine called when the counter 1 has reached the compare value
***************************************************************************************/
ISR (TIMER1_COMPB_vect) {	// Compare B: Post ECHO delay 10mS



	MIP = 0;	// End Measurement
}

/***************************************************************************************
Interrupt service routine called when the counter 1 has reached the compare value
***************************************************************************************/
ISR (TIMER1_COMPA_vect) {	// Compare A : End of Trigger Pulse
	PORTB &= ~(1<<PB1);
	TIMSK1 = (1<<ICIE1)|(1<<TOIE1); // enables the T/C1 Overflow and Capture interrupt;
	TCCR1B = (1<<ICES1);			// Set Positive edge for capture but Don't count yet


}

/******************************************************************************
******************************************************************************/
void Trigger(void) {		// Config Timer 1 for 10 to 15uS pulse.
	if(MIP == 0) {	// Don't allow re-trigger.
		MIP = 1;				// Set Measurement in progress FLAG
		DDRB |= (1<<PB1);		// PB1 as Output for Trigger pulse.
		DDRD &= ~(1<<PB0);		// PB0 as Input for Input Capture (ECHO).
		
		TCNT1 = 0;				// Clear last Echo times.
		ECHOHighPipe = 0;
		
		OCR1B = 10100;			// 10 mS Post echo Delay
		OCR1A = 12;				// 10 us Trigger length.

		PORTB |= (1<<PB1);		// Start Pulse.

		TIFR1 = 0xFF;			//  Clear all timer interrupt flags
		TCCR1A = 0;   // Timer mode with Clear Output on Match
		TCCR1B = (1<<WGM12) | (1<<CS11);  // Counting with CKio/8 CTC Mode enabled
		TIMSK1 = (1<<OCIE1A);	// enables the T/C1 Overflow, Compare A, and Capture interrupt;
	}
	
}

/******************************************************************************
******************************************************************************/
int distance(void) {
	
	init_uart();

	sei();
	
	printf("Hello\n\n");
	int tim, dist;
	int detect = 58;

	while (1 == 1) {
			Trigger();
			while (MIP == 1){};
			tim = (65536*ECHOHigh + ECHOLow) / 2;
			dist = tim /58;
			if (dist < detect) {
				PORTB |= (1<<PORTB2);
				printf("Echo is %d %d cm\n\n",	ECHOLow, dist);
			}
			else {
				PORTB &= ~(1<<PORTB2);
			}
	}
}

