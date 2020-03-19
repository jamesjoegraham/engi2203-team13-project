/*
 * timer.c
 *
 * Created: 3/17/2020 4:54:46 PM
 *  Author: Nick
 */ 

#include "timer.h"

unsigned volatile long int count;

// interrupt for counter
ISR (TIMER1_COMPA_vect)
{
	count++;
}

void initTimer(void)
{
	OCR1A = 250;
	TIMSK1 = (1 << OCIE1A);
	TCCR1B = (1 << CS11) | (1 << CS10) | (1<<WGM12);
	
	sei();
}

unsigned int get_count(void){
	unsigned int cnt_copy;
	cli();
	cnt_copy = count;
	sei();
	return cnt_copy;
}

void set_count(unsigned int cnt){
	cli();
	count = cnt;
	sei();
	return;
}

