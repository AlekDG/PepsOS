#include <sound.h>
#include <stdint.h>
#include <time.h>
#include <lib.h>
// Play sound using built in speaker
static void play_sound(uint32_t nFrequence)
{
	uint32_t Div;
	uint8_t tmp;

	// Set the PIT to the desired frequency
	Div = 1193180 / nFrequence;
	outb(0x43, 0xB6);
	outb(0x42, (uint8_t)(Div));
	outb(0x42, (uint8_t)(Div >> 8));

	tmp = inb(0x61);
	if (tmp != (tmp | 3))
		outb(0x61, tmp | 3);
}

// silence the speaker
static void nosound()
{
	uint8_t tmp = inb(0x61) & 0xFC;
	outb(0x61, tmp);
}

// Make a beep for a specified number of ticks
void beep(uint32_t freq, uint8_t ticks)
{
	play_sound(freq);
	timer_wait(ticks);
	nosound();
}