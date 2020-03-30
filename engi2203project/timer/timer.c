/*
 * timer.c
 *
 * Created: 3/17/2020 4:54:46 PM
 *  Author: Nick
 */ 

#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned volatile long int count;

unsigned volatile long int tcount;

// interrupt for counter
//ISR (TIMER1_OVF_vect)

ISR (TIMER0_OVF_vect)
{
	count++;
	tcount++;
	//TCNT1 = 65520;
	TCNT0 = 240;
}

void initTimer(void)
{/*
	TIMSK1 |= (1 << TOIE1);
	TCNT1 = 65520;
	TCCR1A = 0x00;
	TCCR1B |= (1 << CS10) | (1 << CS12);*/
	TIMSK0 |= (1 << TOIE0);
	TCNT0 = 240;
	TCCR0A = 0x00;
	TCCR0B = (1 << CS10) | (1 << CS12);
	count = 0;
	tcount = 0;
	sei();
}

unsigned long int timems()
{
	return count;
}

unsigned long int timesec()
{
	return (count * 1E3);
}

void timerSet()
{
	tcount = 0;
}

int timermsT(unsigned long int t)
{
	return (t < tcount) ? 1 : 0;
}

int timersecT(unsigned long int t)
{
	t = t * 1E3;
	return (t < tcount) ? 1 : 0;
}

void delayms(int t)
{
	unsigned long int initT = timems();
	while((timems() - initT) < t);
}