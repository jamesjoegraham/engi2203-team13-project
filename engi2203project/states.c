
#include "states.h"
#include "timer/timer.h"
#include "keypad/keypad.h"
#include "admin/admin.h"
#include "sound/sound.h"
#include "halleffect/halleffect.h"
#include "ultrasound/ultrasound.h"


/*
Legend for state
0 = disarmed
1 = arming
2 = armed
3 = person detected
4 = silent alarm
5 = alarming
*/

int curatormode = 0;

void disarmed(void)
{
	// turn on disarmed led
	PORTLED |= (1 << LED1);
	PORTLED &= ~((1 << LED2) | (1 << LED3) | (1 << LED4) | (1 << LED5));
	
	// check for artifact removal
	if(detectHallEffect() && !curatormode)
	{
		state = ST_ALARMING;
	}
	
	resetAttempts();
	
	int k = runKeypad();
	if (k == KP_OK) state = ST_ARMING;
	else if (k == KP_CPASS) curatormode = !curatormode; // toggle curator mode
	else if (k == KP_APASS) state = ST_ADMIN;
}

void arming(void)
{
	timerSet();
	int i = 200;
	while(!timersecT(15)) // delays for 15s
	{
		if(timermsT(i)) // blinks disarmed/arming led with 400ms period
		{
			PORTLED ^= (1 << LED1);  // toggles LED
			beep(50); // beep
			i += 200; // increment by 200ms
		}
		int k = runKeypad();
		if (k == KP_OK) 
		{
			state = ST_DISARMED; // cancel turning on alarm
			return;
		}
	}
	
	beepN(50, 3);
	state = ST_ARMED; // Changes state from arming to armed
	
	PORTLED &= ~(1 << LED1); // turns off arming LED
}

void armed(void)
{
	PORTLED |= (1 << LED2); //turns on armed LED
	
	if(PIND & (1 << PIND4)) //Checks for PIR detection
	{
		//state = ST_ALARMING; // changes state from armed to alarming
		state = ST_DETECTED;
	}
	/*else if(ultraDistance() == 1)
	{
		state = ST_DETECTED;
	}*/
	
	// check for artifact removal
	if(detectHallEffect() && !curatormode)
	{
		state = ST_ALARMING;
	}
	
	int k = runKeypad();
	if (k == KP_DPASS) state = ST_DISARMED;
	else if (k == KP_PPASS) state = ST_PANIC;
	
	
}

void detected(void)
{
	PORTLED &= ~(1 << LED2); // turn off armed LED
	timerSet(0);
	int i = 200;
	while(!timersecT(30))
	{
		if(timermsT(i)) // blinks disarmed/arming led with 400ms period
		{
			PORTLED ^= (1 << LED3);  // toggles LED
			i += 200;
		}
		
		int k = runKeypad();
		if (k == KP_DPASS) state = ST_DISARMED;
		if (k == KP_PPASS) state = ST_PANIC;
		
		if((state == ST_DISARMED) | (state == ST_PANIC)) return;
		
		// check for artifact removal
		if(detectHallEffect() && !curatormode)
		{
			state = ST_ALARMING;
			return;
		}
	}
	
	
	PORTLED &= ~(1 << LED3);
	state = ST_ALARMING; // alarm
}

void panic(void)
{
	PORTLED &= ~((1 << LED2) | (1 << LED3) | (1 << LED4)); // turn off others
	PORTLED |= (1 << LED1) | (1 << LED5); // turn on disarmed led and silent alarm LED
	
	int k = runKeypad();
	if (k == KP_DPASS) state = ST_DISARMED;
}

int alarmOn = 0;
void alarming(void)
{
	if(alarmOn == 0)
	{
		startSong();
		alarmOn = 1;
	}
	PORTLED &= ~((1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED5)); // turn off others
	if(timermsT(50)) // blinks disarmed/arming led with 100ms period
	{
		timerSet();
		PORTLED ^= (1 << LED4);  // toggles LED
	}
	
	playingSong();
	
	int k = runKeypad();
	if (k == KP_DPASS) 
	{
		state = ST_DISARMED;
		alarmOn = 0;
		endSong();
	}
	else if (k == KP_PPASS) 
	{
		state = ST_PANIC;
		alarmOn = 0;
		endSong();
	}
}

int adminmode = 0;
void admin(void)
{
	if (adminmode == 0) // general menu
	{
		if(timermsT(500))
		{
			timerSet();
			PORTLED ^= (1 << LED1);
		}
		char k = getKeypress();
		if(k == '1') adminmode = 1; // dpass
		else if(k == '2') adminmode = 2; // cpass
		else if(k == '3') adminmode = 3; // ppass
		else if(k == '4') adminmode = 4; // factory reset
		else if(k == '5') playSong(); // easter egg
		else if(k == '#')
		{
			adminmode = 0;
			state = 0;
			PORTC = 0x00;
			adminWrite();
			initKeypad();
		}
	}
	else if (adminmode == 1) // dpass edit
	{
		PORTLED &= ~(1 << LED1);
		if(timermsT(500))
		{
			timerSet();
			PORTLED ^= (1 << LED2);
		}
		int k = runCode();
		if (k == 1) // # pressed
		{
			char c[8];
			int l = getCodeStr(c);
			setDPASS(c, l);
			adminmode = 0;
		}
	}
	else if (adminmode == 2) // cpass edit
	{
		PORTLED &= ~(1 << LED1);
		if(timermsT(500))
		{
			timerSet();
			PORTLED ^= (1 << LED3);
		}
		int k = runCode();
		if (k == 1) // # pressed
		{
			char c[8];
			int l = getCodeStr(c);
			setCPASS(c, l);
			adminmode = 0;
		}
	}
	else if (adminmode == 3) // ppass edit
	{
		PORTLED &= ~(1 << LED1);
		if(timermsT(500))
		{
			timerSet();
			PORTLED ^= (1 << LED4);
		}
		int k = runCode();
		if (k == 1) // # pressed
		{
			char c[8];
			int l = getCodeStr(c);
			setPPASS(c, l);
			adminmode = 0;
		}
	}
	else if (adminmode == 4) // factory reset
	{
		if(timermsT(500))
		{
			timerSet();
			PORTLED ^= 0xFF;
		}
		char k = getKeypress();
		if (k == '7')
		{
			adminReset();
			adminmode = 0;
		}
	}
}