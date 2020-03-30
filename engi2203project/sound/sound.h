/*
 * sound.h
 *
 * Created: 2020-02-22 11:24:18 PM
 *  Author: james
 */ 


#ifndef SOUND_H_
#define SOUND_H_

#define SOUNDPORT PORTC
#define SOUNDPORTO PORTC5
#define SOUNDPORTDD DDRC


// play a short tone
void beepTone(int t);

void beep(int t);

void beepN(int t, int n);

void playSong();

void startBeep(int freq);
void endBeep();
void toggleBeep(int freq);

void startSong();
void playingSong();
void endSong();


#endif /* SOUND_H_ */