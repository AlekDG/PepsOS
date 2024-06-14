
#include <UserSyscalls.h>

#define TIME_ZONE -3

void timeToStr(char *dest)
{
      unsigned int s = call_seconds();
      unsigned int m = call_minutes();
      unsigned int h = call_hours() + TIME_ZONE;

      if (h < 0)
      {
            h += 24;
      }

      dest[0] = (h / 10) % 10 + '0';
      dest[1] = h % 10 + '0';
      dest[2] = ':';
      dest[3] = (m / 10) % 10 + '0';
      dest[4] = m % 10 + '0';
      dest[5] = ':';
      dest[6] = (s / 10) % 10 + '0';
      dest[7] = s % 10 + '0';
      dest[8] = '\0';
}

void dateToStr(char *dest)
{
      unsigned int d = call_day();
      unsigned int m = call_month();
      unsigned int y = call_year();

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
