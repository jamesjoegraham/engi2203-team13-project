/*
 * engi2203project.c
 *
 * Created: 2020-02-11 2:46:10 PM
 * Author : james
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "4x3_Matrix.h"
#include "timer.h"
#include "states.h"
#include "sound/sound.h"
#include "alarm/alarm.h"





int main(void)
{
	// initialize Keypad & timer
	initKeypad();
	initTimer();
	set_count(0);
	
	// initialize LED ports
	DDRC |= 0xFF;
	
	// initialize state
	state = 0;
	
	// main loop
    while (1) 
    {
		if(state == 0) // Runs in disarmed state
		{
			disarmed();
		}
		if(state == 1) // Runs while system is arming
		{
			arming();
		}
		if(state == 2) // runs while system is armed
		{
			armed();
		}
		if(state == 3) // runs while system is alarming
		{
			alarming();
		}
    }
}

