/*
 * admin.c
 *
 * Created: 2020-03-29 5:08:52 PM
 *  Author: james
 */ 

#include <avr/eeprom.h>
#include <string.h>
#include <stdio.h>

#define EEPROMREGISTER 12

#include "../keypad/keypad.h"

char default_dpass[] = "6969"; // regular password

char default_cpass[] = "1234"; // curator password

char default_ppass[] = "4202"; // panic mode password

struct code 
{
	int codelength;
	char codechars[8];
};

struct codes
{
	char placeholder;
	struct code dpass_code;
	struct code cpass_code;
	struct code ppass_code;
};

struct codes currentCodes;

void adminDump()
{
	printf("In struct currentCodes,\n");
	printf("The placeholder is %c\n\n", currentCodes.placeholder);
	
	printf("The dpass has length %d with content %s\n", currentCodes.dpass_code.codelength, currentCodes.dpass_code.codechars);
	printf("The default dpass is %s\n\n", default_dpass);
	
	printf("The cpass has length %d with content %s\n", currentCodes.cpass_code.codelength, currentCodes.cpass_code.codechars);
	printf("The default cpass is %s\n\n", default_cpass);
	
	printf("The ppass has length %d with content %s\n", currentCodes.ppass_code.codelength, currentCodes.ppass_code.codechars);
	printf("The default ppass is %s\n\n", default_ppass);
}

void adminReset()
{
	struct codes p;
	p.placeholder = '*';
	
	strcpy(p.dpass_code.codechars, default_dpass);
	p.dpass_code.codelength = sizeof(default_dpass) - 1;
	
	strcpy(p.cpass_code.codechars, default_cpass);
	p.cpass_code.codelength = sizeof(default_cpass) - 1;
	
	strcpy(p.ppass_code.codechars, default_ppass);
	p.ppass_code.codelength = sizeof(default_ppass) - 1;
	
	eeprom_write_block((const void*)&p, (void*)EEPROMREGISTER, sizeof(p)); // write to eeprom
	currentCodes = p;
}


void adminInit()
{
	eeprom_is_ready();
	struct codes passcodes;
	eeprom_read_block((void*)&passcodes, (const void*)EEPROMREGISTER, sizeof(passcodes));
	
	if (passcodes.placeholder != '*') // if there are no loaded codes
	{
		printf("Failed to load codes!\n");
		adminReset();
	}
	else
	{
		printf("Successfully loaded codes!\n");
		currentCodes = passcodes;
	}
	
	adminDump();
}

void adminWrite()
{
	eeprom_write_block((const void*)&currentCodes, (void*)EEPROMREGISTER, sizeof(currentCodes));	
	printf("Codes written to EEPROM\n");
}

int getDPASS(char* c)
{
	strcpy(c, currentCodes.dpass_code.codechars);
	return currentCodes.dpass_code.codelength;
}

void setDPASS(char* c, int n)
{
	strcpy(currentCodes.dpass_code.codechars, c);
	currentCodes.dpass_code.codelength = n;
	printf("DPASS Set to %s", c);
}

int getCPASS(char* c)
{
	strcpy(c, currentCodes.cpass_code.codechars);
	return currentCodes.cpass_code.codelength;
}

void setCPASS(char* c, int n)
{
	strcpy(currentCodes.cpass_code.codechars, c);
	currentCodes.cpass_code.codelength = n;
	printf("CPASS Set to %s", c);
}

int getPPASS(char* c)
{
	strcpy(c, currentCodes.ppass_code.codechars);
	return currentCodes.ppass_code.codelength;
}

void setPPASS(char* c, int n)
{
	strcpy(currentCodes.ppass_code.codechars, c);
	currentCodes.ppass_code.codelength = n;
	printf("PPASS Set to %s", c);
}