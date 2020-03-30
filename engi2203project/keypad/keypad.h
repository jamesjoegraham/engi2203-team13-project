
/*
 * 4x3_Matrix.h
 *
 * Created: 2/19/2020 7:24:54 PM
 *  Author: NickCestnick
 */ 

#ifndef KEYPAD_H
#define KEYPAD_H

#define ROW1 3
#define ROW2 2
#define ROW3 1
#define ROW4 0
#define COL1 7
#define COL2 6
#define COL3 5

#define ROWPORT PORTB
#define COLPORT PORTD

#define COLPIN PIND

#define ROWDDR DDRB
#define COLDDR DDRD

#define KP_OK -1
#define KP_DPASS 1
#define KP_CPASS 2
#define KP_PPASS 3
#define KP_APASS 4


//initialize variable for key pressed

void initKeypad(void);

void resetAttempts();

char getKeypress(void);

int runCode();

int getCodeStr(char* c);

int runKeypad(); // runs keypad and returns code value

#endif
