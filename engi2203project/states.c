
#include "states.h"
#include "timer.h"


/*
Legend for state
0 = disarmed
1 = arming
2 = armed
3 = person detected
4 = silent alarm
5 = alarming
*/

unsigned int stemp = 0;  // for blinking LED funtionality

int curatormode = 0;

int pinmatch = 0;

char keypress = 0;

/*
Legend for pinmatch
0 = no maching pin
1 = disarmed
2 = museum curator
3 = panic code
*/

void disarmed(void)
{
	// turn on disarmed led
	PORTLED |= (1<<LED1);
	char keypress = getNewKeypress(); // get keypress
	
	if(keypress == '#') // check if # was pressed
	{
		state = 1; // change state from disarmed to arming
	}
	else if(keypress) // check if anything else was pressed
	{
		pinmatch = updatePin(keypress); // updates pin acordingly
		if(pinmatch == 2) // check is curator code was entered
		{
			curatormode = 1;  // activates curator mode
		}
		pinmatch = 0; // resets pinmatch
	}
	
}

void arming(void)
{
	set_count(0);  // resets counter variable
	stemp = 0;
	while(get_count() < 5000) // delays for 15s
	{
		if((get_count() - stemp) >= 200) // blinks disarmed/arming led with 400ms period
		{
			PORTLED ^= (1<<LED1);  // toggles LED
			stemp = get_count();
		}
	}
	
	state = 2; // Changes state from arming to armed
	
	PORTLED &= ~(1<<LED1); // turns off arming LED
}

void armed(void)
{
	PORTLED |= (1<<LED2); //turns on armed LED
	if(PIND & (1<<PIND4)) //Checks for PIR detection
	{
		state = 3; // changes state from armed to alarming
	}
}

void detected(void)
{
	set_count(0);
	stemp = 0;
	while(get_count() < 30000)
	{
		if((get_count() - stemp) >= 200) // blinks disarmed/arming led with 400ms period
		{
			PORTLED ^= (1<<LED2);  // toggles LED
			stemp = get_count();
		}
		
		keypress = getNewKeypress();
		
		if(keypress)
		{
			pinmatch = updatePin(keypress);
			if(pinmatch == 1)
			{
				state = 0;
			}
			else if(pinmatch == 3)
			{
				state = 4;
			}
			
			pinmatch = 0;
		}
		
		if((state == 0) | (state == 4)) break;
	}
	
	if(state == 3) state = 5;
	PORTLED &= ~(1<<LED2); //turn off armed LED
}

void panic(void)
{
	PORTLED |= (1<<LED1) | (1<<LED4); // turn on disarmed led and silent alarm LED
}

void alarming(void)
{
	PORTLED |= (1<<LED3);
}