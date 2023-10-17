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

void timeToStr(char *dest)
{
      uint8_t s = seconds();
      uint8_t m = minutes();
      uint8_t h = hours() - 3;

      dest[0] = (h / 10) % 10 + '0';
      dest[1] = h % 10 + '0';
      dest[2] = ':';
      dest[3] = (m / 10) % 10 + '0';
      dest[4] = m % 10 + '0';
      dest[5] = ':';
      dest[6] = (s / 10) % 10 + '0';
      dest[7] = s % 10 + '0';
}

void dateToStr(char *dest)
{
      uint8_t d = day();
      uint8_t m = month();
      uint8_t y = year();

      dest[0] = (d / 10) % 10 + '0';
      dest[1] = d % 10 + '0';
      dest[2] = '/';
      dest[3] = (m / 10) % 10 + '0';
      dest[4] = m % 10 + '0';
      dest[5] = '/';
      dest[6] = (y / 10) % 10 + '0';
      dest[7] = y % 10 + '0';
}

void getDate(char *dest)
{
      timeToStr(dest);
      dest[8] = ' ';
      dateToStr(dest + 9);
}
