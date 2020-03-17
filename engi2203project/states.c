
#include "states.h"

void disarmed(void)
{
	// turn on disarmed led
	PORTLED |= (1<<LED1);
	char keypress = getNewKeypress(); // get keypress
	
	if(keypress == '#') // check if # was pressed
	{
		state = 1; // change state from disarmed to arming
	}
	
}

void arming(void)
{
	set_count(0);  // resets counter variable
	temp = 0;
	while(get_count() < 15000) // delays for 15s
	{
		if((get_count() - temp) >= 200) // blinks disarmed/arming led with 400ms period
		{
			PORTLED ^= (1<<LED1);
			temp = get_count();
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
		PORTLED &= ~(1<<LED2); //turn off armed LED
	}
}

void alarming(void)
{
	PORTLED |= (1<<LED3); //turn on alarming LED 
}