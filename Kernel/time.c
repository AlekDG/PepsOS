#include <time.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

void timer_wait(int ticks)
{
	int startTime=ticks_elapsed();
	while(ticks_elapsed()-startTime<ticks){
	}
}
