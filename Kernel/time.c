#include <time.h>

extern void _hlt();

static unsigned long ticks = 0;

void timer_handler()
{
	ticks++;
}

int ticks_elapsed()
{
	return ticks;
}

int seconds_elapsed()
{
	return ticks / 18;
}

void timer_wait(int time)
{
	int startTime = ticks;
	while (ticks - startTime < time)
	{
		_hlt();
	}
}
