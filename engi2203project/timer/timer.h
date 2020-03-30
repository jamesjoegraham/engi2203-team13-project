#ifndef TIMER_H
#define TIMER_H

void initTimer(void);

unsigned long int timeu();

unsigned long int timems();

unsigned long int timesec();

void timerSet();

int timermsT(unsigned long int t);

int timersecT(unsigned long int t);

void delayms(int t);

#endif