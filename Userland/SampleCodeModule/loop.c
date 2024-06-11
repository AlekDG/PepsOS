
#include <UserSyscalls.h>
#include <menu.h>

#define TICKS 10
#define START_POSITION 100

void loop() {
  int myPid = call_getPid();
  int x;
  int y;
  while (1) {
    // x = call_getXBuffer();
    // y = call_getYBuffer();
    call_setXBuffer(START_POSITION);
    call_setYBuffer(START_POSITION);
    call_drawFace(0, 0, 100);
    call_drawStringFormatted("Buenas Buenas! Aqui el proceso loop con ID ",
                             WHITE, PEPSIBLUE, 2);
    call_printInteger(myPid);
    call_drawStringFormatted(" molestando, bueno me voy a dormir. Chau! ",
                             WHITE, PEPSIBLUE, 2);
    // call_setXBuffer(x);
    // s call_setYBuffer(y);
    call_sleep(TICKS);
  }
}