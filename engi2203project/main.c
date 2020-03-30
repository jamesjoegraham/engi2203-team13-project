// main.c

#define DEBUGMODE 1 // prints stuff out to uart
#define ADMINMODE 1 // defines if admin mode is on

#include <avr/io.h>
#include <avr/interrupt.h>

#include "keypad/keypad.h"
#include "timer/timer.h"
#include "states.h"
#include "sound/sound.h"
#include "admin/admin.h"

#include "uart_utility_functions.h"

/*
Legend for state
0 = disarmed
1 = arming
2 = armed
3 = person detected
4 = silent alarm
5 = alarming
*/



int main(void)
{
	// initialize Keypad & timer
	if (DEBUGMODE) init_uart();
	if (ADMINMODE) adminInit();
	initKeypad();
	initTimer();
	
	// initialize LED ports
	DDRC |= (1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4) | (1 << LED5);
	
	// initialize state
	state = ST_DISARMED;
	
	// main loop
	
	
    while (1) 
    {
		if(state == ST_DISARMED) // Runs in disarmed state
		{
			disarmed();
		}
		if(state == ST_ARMING) // Runs while system is arming
		{
			arming();
		}
		if(state == ST_ARMED) // runs while system is armed
		{
			armed();
		}
		if(state == ST_DETECTED) // runs while system has detected something (30s)
		{
			detected();
		}
		if(state == ST_PANIC) // runs while system is in panic mode
		{
			panic();
		}
		if(state == ST_ALARMING)
		{
			alarming();
		}
		if(state == ST_ADMIN)
		{
			admin();
		}
    }
}

