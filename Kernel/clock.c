#include <clock.h>
#include <lib.h>

#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 0x04
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
