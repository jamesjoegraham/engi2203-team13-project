/*
 * sound.c
 *
 * Created: 2020-02-22 11:26:34 PM
 *  Author: james
 */ 

#include <avr/io.h>
#include <util/delay.h>

#include "sound.h"

#define soundPort PORTB
#define soundPin 0

// frequency of beep in hertz
const int beepFrequency = 2000;


void beepTone(int t)
{
	float bP = 1. / (float)beepFrequency;
	int n = (int)(beepFrequency * (float)t / 1000.);
	
	for (int i = 0; i < n; i++)
	{
		soundPort |= (1 << soundPin);
		_delay_us((int)(bP * 10E6 / 2));
		soundPort &= ~(1 << soundPin);
		_delay_us((int)(bP * 10E6 / 2));
	}
	
	
	
}
