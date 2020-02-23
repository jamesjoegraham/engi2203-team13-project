/*
 * alarm.c
 *
 * Created: 2020-02-23 4:34:02 PM
 *  Author: james
 */ 

#include <avr/io.h>

#define aPORT PORTC
#define L1 0
#define L2 1
#define L3 2
#define L4 3

void disarm()
{
	aPORT |= (1 << L1);
	aPORT &= ~((1 << L2) | (1 << L3) | (1 << L4));
}

void arm()
{
	aPORT |= (1 << L2);
	aPORT &= ~((1 << L1) | (1 << L3) | (1 << L4));
}

void pdetect()
{
	aPORT |= (1 << L3);
}

void alarm(int cnt)
{
	if ((int)((double)cnt / 500) % 2) {
		aPORT |= (1 << L4);
	}
	else {
		aPORT &= ~(1 << L4);
	}
}