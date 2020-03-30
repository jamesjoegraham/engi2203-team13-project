#ifndef STATES_H
#define STATES_H

#include <avr/io.h>



#define PORTLED PORTC

#define LED1 PORTC0 // disarmed / arming
#define LED2 PORTC1 // armed
#define LED3 PORTC2 // detected
#define LED4 PORTC3 // alarming
#define LED5 PORTC4 // silent alarm

#define ST_DISARMED 0
#define ST_ARMING 1
#define ST_ARMED 2
#define ST_DETECTED 3
#define ST_PANIC 4
#define ST_ALARMING 5
#define ST_ADMIN 6

int state; // variable to define the system's current state

void disarmed(void);

void arming(void);

void armed(void);

void detected(void);

void panic(void);

void alarming(void);

void admin(void);

#endif