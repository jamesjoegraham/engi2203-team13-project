#ifndef STATES_H
#define STATES_H

#include <avr/io.h>



#define PORTLED PORTC

#define LED1 PORTC0 // disarmed / arming
#define LED2 PORTC1 // armed
#define LED3 PORTC2 // alarming
#define LED4 PORTC3 // silent alarm

int state; // variable to define the syustems current state

void disarmed(void);

void arming(void);

void armed(void);

void detected(void);

void panic(void);

void alarming(void);

#endif