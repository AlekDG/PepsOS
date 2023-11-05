#ifndef USER_SOUND_H
#define USER_SOUND_H
#include <stdint.h>

void playSound(uint32_t freq, uint32_t ticks);
void playEndingSound();
void playStartingSound();
void playMegalovania();

#endif