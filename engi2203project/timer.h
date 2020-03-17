

#ifndef TIMER_H
#define TIMER_H

#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000UL

unsigned volatile long int count;


void initTimer(void);

unsigned int get_count(void);

void set_count(unsigned int cnt);



#endif