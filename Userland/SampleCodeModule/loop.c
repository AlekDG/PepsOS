#include <io.h>
#include <UserSyscalls.h>
#include <menu.h>

#define TICKS 10
#define START_POSITION 100

void loop() {
  int myPid = call_getPid();

  while (1) {

    call_setXBuffer(START_POSITION);
    call_setYBuffer(START_POSITION);
    call_drawFace(0, 0, 100);
    call_drawStringFormatted("Buenas Buenas! Aqui el proceso loop con ID ",
                             WHITE, PEPSIBLUE, 2);
    call_printInteger(myPid);
    call_drawStringFormatted(" molestando, bueno me voy a dormir. Chau! ",
                             WHITE, PEPSIBLUE, 2);
    call_sleep(TICKS);
  }
}