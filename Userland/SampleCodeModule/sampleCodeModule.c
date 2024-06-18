// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* sampleCodeModule.c */
#include <menu.h>
#include <pepsiman.h>
#include <snake.h>
#include <snake2.h>
#include <sound.h>
#include <stdint.h>

int main()
{
	playStartingSound();
	while(1){
		drawMenu();
	}

	return 0;
}