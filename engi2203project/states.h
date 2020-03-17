#ifndef STATES_H
#define STATES_H

#include <avr/io.h>
#include "4x3_Matrix.h"
#include "timer.h"

#define PORTLED PORTC

#define LED1 PORTC0 // disarmed / arming
#define LED2 PORTC1 // armed
#define LED3 PORTC2 // alarming

int state; // variable to define the syustems current state

/*
Legend for state
0 = disarmed
1 = arming
2 = armed
3 = alarming
*/

unsigned int temp;  // for blinking LED funtionality

void disarmed(void);

void arming(void);

void armed(void);

void alarming(void);

#endif