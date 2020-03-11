
/*
 * 4x3_Matrix.h
 *
 * Created: 2/19/2020 7:24:54 PM
 *  Author: NickCestnick
 */ 

#ifndef MATRIX_H
#define MATRIX_H

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




void initHardware(void);

void setRowLow(int row);

int colPushed(void);

char getKeypress(void);

char getNewKeypress(void);




#endif