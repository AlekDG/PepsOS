#include <clock.h>
#include <lib.h>

#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 0x04
#define DAY 0x07
#define MONTH 0x08
#define YEAR 0x09
#define TIME_ZONE -3

unsigned char clock(unsigned char mode); // esta en clock.asm

unsigned int decode(unsigned char time)
{
      return (time >> 4) * 10 + (time & 0x0F);
}

unsigned int seconds()
{
      return decode(clock(SECONDS));
}

unsigned int minutes()
{
      return decode(clock(MINUTES));
}

unsigned int hours()
{
      return decode(clock(HOURS));
}

unsigned int day()
{
      return decode(clock(DAY));
}

unsigned int month()
{
      return decode(clock(MONTH));
}

unsigned int year()
{
      return decode(clock(YEAR));
}