/*
 * 4x3_Matrix.col
 *
 * Created: 2/19/2020 7:24:54 PM
 *  Author: NickCestnick
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include "4x3_Matrix.h"

char buttons[4][3] = {{'1', '2', '3'},
					  {'4', '5', '6'},
					  {'7', '8', '9'},
					  {'*', '0', '#'}};
						  
char keypress;

char upass[] = "****";

char dpass[] = "6969";

char cpass[] = "1234";

char ppass[] = "4202";

int pindex = 0;

void initKeypad(void)
{
	ROWPORT &= ~(1<<ROW1) & ~(1<<ROW2) & ~(1<<ROW3) & ~(1<<ROW4); //Set Row Ports low
	COLPORT |= (1<<COL1) | (1<<COL2) | (1<<COL3); //Sets Column Ports high
}

void setRowLow(int row)
{
	ROWDDR &=  ~(1<<ROW1) & ~(1<<ROW2) & ~(1<<ROW3) & ~(1<<ROW4); // Sets all Rows to Input
		
	if(row == 0)
	{
		ROWDDR |= (1<<ROW1); // Sets Row 1 to Output
	}else if(row == 1)
	{
		ROWDDR |= (1<<ROW2); // Sets Row 2 to Output
	}else if(row == 2)
	{
		ROWDDR |= (1<<ROW3); // Sets Row 3 to Output
	}else
	{
		ROWDDR |= (1<<ROW4); // Sets Row 4 to Output
	}
}

int colPushed(void)
{
	if((COLPIN & (1<<COL1)) == 0) //Checks if Column 1 is low
	{
		return 1;
	}
	else if((COLPIN & (1<<COL2)) == 0) //Checks if Column 2 is low
	{
		return 2;
	}
	else if((COLPIN & (1<<COL3)) == 0) //Checks if Column 3 is low
	{
		return 3;
	}
	
	
	return 0; //returns nothing if no col detected
}

char getKeypress(void)
{
	for(int row = 0; row < 4; row++){
		setRowLow(row);
		_delay_ms(20);
		
		int col = colPushed();
		
		if(col){
			return buttons[row][col-1];
		}
	}
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
 

int updatePin(char key)
{
	if(key == '*')
	{
		pindex = 0;
		upass[0] = '*';
		upass[1] = '*';
		upass[2] = '*';
		upass[3] = '*';
	}else if(key)
	{
		upass[pindex] = key;
		pindex++;
	}
	
	if(pindex == 4)
	{
		if(strcmp(upass, dpass) == 0)
		{
			upass[0] = '*';
			upass[1] = '*';
			upass[2] = '*';
			upass[3] = '*';
			pindex = 0;
			return 1;
		}
		else if(strcmp(upass, cpass) == 0)
		{
			upass[0] = '*';
			upass[1] = '*';
			upass[2] = '*';
			upass[3] = '*';
			pindex = 0;
			return 2;
		}
		else if(strcmp(upass, ppass) == 0)
		{
			upass[0] = '*';
			upass[1] = '*';
			upass[2] = '*';
			upass[3] = '*';
			pindex = 0;
			return 3;
		}
		else
		{
			upass[0] = '*';
			upass[1] = '*';
			upass[2] = '*';
			upass[3] = '*';
			pindex = 0;
			return 0;
		}
		
	}
	
	return 0;
}