/*
 * 4x3_Matrix.col
 *
 * Created: 2/19/2020 7:24:54 PM
 *  Author: NickCestnick
 */ 
#include <stdio.h>
#include <avr/io.h>
#include <string.h>
#include "keypad.h"
#include "../sound/sound.h"
#include "../timer/timer.h"
#include "../uart_utility_functions.h"
#include "../admin/admin.h"

const char buttons[4][3] = {{'1', '2', '3'},
					  {'4', '5', '6'},
					  {'7', '8', '9'},
					  {'*', '0', '#'}};

int pindex = 0;
char uentry[8];

char dpass[] = "6969"; // regular password

char cpass[] = "1234"; // curator password

char ppass[] = "4202"; // panic mode password

char apass[] = "1111"; // admin password

int attempts = 0;

void initKeypad(void)
{
	ROWPORT &= ~((1 << ROW1) | (1 << ROW2) | (1 << ROW3) | (1 << ROW4)); //Set Row Ports low
	COLPORT |= (1 << COL1) | (1 << COL2) | (1 << COL3); //Sets Column Ports high
	
	// get new passwords
	char dp[8], cp[8], pp[8];
	getDPASS(dp);
	getCPASS(cp);
	getPPASS(pp);
	strcpy(dpass, dp);
	strcpy(cpass, cp);
	strcpy(ppass, pp);
}

void setRowLow(int row)
{
	ROWDDR &=  ~((1<<ROW1) | (1<<ROW2) | (1<<ROW3) | (1<<ROW4)); // Sets all Rows to Input
		
	if(row == 0)
	{
		ROWDDR |= (1 << ROW1); // Sets Row 1 to Output
	} 
	else if(row == 1)
	{
		ROWDDR |= (1 << ROW2); // Sets Row 2 to Output
	}
	else if(row == 2)
	{
		ROWDDR |= (1 << ROW3); // Sets Row 3 to Output
	}
	else
	{
		ROWDDR |= (1 << ROW4); // Sets Row 4 to Output
	}
}

int colPushed(void)
{
	if((COLPIN & (1 << COL1)) == 0) //Checks if Column 1 is low
	{
		return 1;
	}
	else if((COLPIN & (1 << COL2)) == 0) //Checks if Column 2 is low
	{
		return 2;
	}
	else if((COLPIN & (1 << COL3)) == 0) //Checks if Column 3 is low
	{
		return 3;
	}
	
	
	return 0; //returns nothing if no col detected
}

unsigned long int debounce = 0;
char getKeypress(void)
{
	if ((timems() - debounce) < 200) return 0;
	debounce = timems();
	
	for(int row = 0; row < 4; row++){
		setRowLow(row);
		delayms(5);
		
		int col = colPushed();
		
		if(col){
			// play a sound
			printf("Key %c pressed\n", buttons[row][col-1]);
			beep(50);
			return buttons[row][col-1];
		}
	}
	return 0;
}

void resetAttempts()
{
	attempts = 0;
}

char getNewKeypress(void)
{
	
	static char last_button;
	char b = getKeypress();
	
	//Check if we held button down
	if(b == last_button) return 0;
	
	last_button = b;
	
	return b;
}

void clearKey()
{
	pindex = 0; // sets index to 0
	memset(uentry, 0, sizeof(uentry)); // clears uentry string
}

int runCode()
{
	char key = getKeypress();
	
	if(pindex >= 8) clearKey();
	
	if(key == '*') // clear
	{
		clearKey();
	}
	else if((key == '#') && (pindex > 0))
	{
		return 1;
	}
	else if(key)
	{
		uentry[pindex] = key;
		pindex++;
	}
	return 0;
}

int getCodeStr(char* c)
{
	strcpy(c, uentry);
	return pindex;
	clearKey();
}

int runKeypad()
{
	char key = getKeypress();
	
	if(pindex >= 8) clearKey(); // reset if too many keys entered
	
	if(key == '*') // clear
	{
		clearKey();
		return 0;
	}
	else if(key == '#') // OK
	{
		int ret = 0;
		if(strcmp(uentry, dpass) == 0)
		{
			printf("dpass\n");
			ret = KP_DPASS;
		}
		else if(strcmp(uentry, cpass) == 0)
		{
			printf("cpass\n");
			ret = KP_CPASS;
		}
		else if(strcmp(uentry, ppass) == 0)
		{
			printf("ppass\n");
			ret = KP_PPASS;
		}
		else if(strcmp(uentry, apass) == 0)
		{
			printf("apass\n");
			ret = KP_APASS;
		}
		else if (pindex == 0)
		{
			ret = KP_OK;
		}
		attempts++;
		if (attempts > 4)
		{
			ret = 0;
		}
		clearKey();
		return ret;
	}
	else if(key) // valid keypress
	{
		uentry[pindex] = key;
		pindex++;
	}
	return 0;
}
