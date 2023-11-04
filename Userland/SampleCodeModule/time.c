#include <stdint.h>
#include <time.h>

void timeToStr(char *dest)
{
      uint8_t s = call_seconds();
      uint8_t m = call_minutes();
      uint8_t h = call_hours() - 3;

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
      uint8_t d = call_day();
      uint8_t m = call_month();
      uint8_t y = call_year();

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
