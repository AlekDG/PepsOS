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