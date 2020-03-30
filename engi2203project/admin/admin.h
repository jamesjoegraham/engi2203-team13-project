/*
 * admin.h
 *
 * Created: 2020-03-29 5:05:08 PM
 *  Author: james
 */ 


#ifndef ADMIN_H_
#define ADMIN_H_

// CODE DEFAULTS

void adminInit();

int getDPASS(char* c);
void setDPASS(char* c, int n);

int getCPASS(char* c);
void setCPASS(char* c, int n);

int getPPASS(char* c);
void setPPASS(char* c, int n);

void adminWrite();

void adminReset();

#endif /* ADMIN_H_ */