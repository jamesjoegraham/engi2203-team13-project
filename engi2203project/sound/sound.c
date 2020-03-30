/*
 * sound.c
 *
 * Created: 2020-02-22 11:26:34 PM
 *  Author: james
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sound.h"
#include "../timer/timer.h"

#define soundPort PORTD
#define soundPin PORTD4

// frequency of beep in hertz
const int beepFrequency = 2000;

/*
void beepTone(int t)
{
	float bP = 1. / (float)beepFrequency;
	int delayTime = (bP * 10E6 / 2.);
	
	unsigned long int initTime = timems();
	while ((timems() - initTime) < (unsigned long int)t)
	{
		soundPort |= (1 << soundPin);
		soundPort &= ~(1 << soundPin);
	}
	
	return 1;
	
}*/

volatile int beepT = 0;
ISR (TIMER1_COMPA_vect)
{
	if(beepT)
	{
		SOUNDPORT |= (1 << SOUNDPORTO);
		//SOUNDPORTDD |= (1 << SOUNDPORTO);
	}
	else
	{
		SOUNDPORT &= ~(1 << SOUNDPORTO);
		//SOUNDPORTDD &= ~(1 << SOUNDPORTO);
	}
	beepT = !beepT;
	TCNT1 = 0;
}

int beepToggle = 0;
void toggleBeep(int freq)
{
	if (beepToggle) startBeep(freq);
	else endBeep();
	beepToggle = !beepToggle;
}

void startBeep(int freq)
{
	float period = 1. / (float)freq;
	short int ocrp = (int)(period * 1E6 * 2);
	memcpy((void *)&OCR1A, &ocrp, sizeof(ocrp));
	TIFR1 = 0xFF;
	SOUNDPORTDD |= (1 << SOUNDPORTO);
	SOUNDPORT = (1 << SOUNDPORTO);
	
	
	TCCR1A = 0x00;
	TCCR1B = (1 << WGM12) | (1 << CS11);
	TIMSK1 = (1<<OCIE1A);
	TCNT1 = 0;
	
}

void endBeep()
{
	TCCR1B = 0x00;
	SOUNDPORTDD &= ~(1 << SOUNDPORTO);
	SOUNDPORT &= ~(1 << SOUNDPORTO);
}

void dlayu(const int t)
{
	for (int i = 0; i < t; i++)
	{
		_delay_us(1);
	}
}

void dlaym(const int t)
{
	for (int i = 0; i < t; i++)
	{
		_delay_ms(1);
	}
}

void beep(int t)
{
	SOUNDPORTDD |= (1 << SOUNDPORTO);
	unsigned long int initT = timems();
	while((timems() - initT) < t)
	{
		SOUNDPORT ^= (1 << SOUNDPORTO);
		_delay_us(500);
	}
}

void beepN(int t, int n)
{
	for (int i = 0; i < n; i++)
	{
		beep(t);
		dlaym(t);
	}
}

void beepHz(int f, int t)
{
	SOUNDPORTDD |= (1 << SOUNDPORTO);
	unsigned long int initT = timems();
	int waitP = (int)(1. / (double)f * 1E6 / 2.);
	
	while((timems() - initT) < t)
	{
		SOUNDPORT ^= (1 << SOUNDPORTO);
		dlayu(waitP);
	}
	SOUNDPORT &= ~(1 << SOUNDPORTO);
	SOUNDPORTDD &= ~(1 << SOUNDPORTO);
}

// notes            A      a       B       C       c      D        d      E        F       f       G       g
float notes[12] = {440.00, 466.16, 493.88, 523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61};
char noteL[12] = {'A', 'a', 'B', 'C', 'c', 'D', 'd', 'E', 'F', 'f', 'G', 'g'};
 

int getFreqFromNote(const char note[2])
{
	char base = note[0];
	int num = note[1] - '0'; // c hack
	
	int i = 0;
	while (noteL[i] != base) i++; // get proper note
	
	float basefreq = notes[i];
	basefreq = basefreq * pow(2, num - 4);

	return (int)basefreq;
}


struct notePair {
	char note[2];
	int nsize; // how much it is divided by (e.g. 1 = whole note, 2 = half note, etc)
	};


// take on me - aha
struct notePair song[24] = {{"f5", 8}, {"f5", 8}, {"D5", 8}, {"B5", 4}, {"B5", 4}, {"E5", 4}, {"E5", 4}, {"E5", 8}, {"g5", 8}, {"g5", 8}, {"A6", 8}, {"B6", 8},
							 {"A6", 8}, {"A6", 8}, {"A6", 8}, {"E5", 4}, {"D5", 4}, {"f5", 4}, {"f5", 4}, {"f5", 8}, {"E5", 8}, {"E5", 8}, {"f5", 8}, {"E5", 8}};

int songLen = 24;
int tempo = 169;
void playSong()
{
	// play staccato (half on, half rest)
	
	float beat = 60. / (float)tempo * 4;
	
	for (int j = 0; j < 2; j++) // play it twice!
	{
		for (int i = 0; i < songLen; i++)
		{
			float delayTime = beat / (float)song[i].nsize * 1000. / 2;
			beepHz(getFreqFromNote(song[i].note), (int)delayTime);
			dlaym((int)delayTime);
		}
	}
}

int playingSongNote = 0;
int songStart = 0;
unsigned long int playingSongNoteStart = 0;
void startSong()
{
	playingSongNote = 0;
	songStart = 1;
}

void endSong()
{
	endBeep();
}

void playingSong()
{
	float beat = 60. / (float)tempo;
	float noteLen = beat * 4. / (float)song[playingSongNote].nsize * 1000.;
	
	if (playingSongNote >= 24)
	{
		startSong();
	}
	else if ((timems() - playingSongNoteStart) > noteLen) // note is complete
	{
		playingSongNote++;
		playingSongNoteStart = timems();
		startBeep(getFreqFromNote(song[playingSongNote].note));
	}
	else if((timems() - playingSongNoteStart) > (noteLen / 2))
	{
		endBeep();
	}
	else if(songStart)
	{
		playingSongNoteStart = timems();
		songStart = 0;
		startBeep(getFreqFromNote(song[playingSongNote].note));
	}
}