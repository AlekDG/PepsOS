#include <UserSyscalls.h>
#define NULL 0

void ps() {
  int processCount;
  processInfo *processList = call_ps(&processCount);
  call_drawRectangle(0x000000, 0, 0, call_getWidth(), call_getHeight());

  for (int i = 0; i <= processCount; i++) {
    // imprimo proceso
    call_drawStringFormatted(processList[i].name, 0xFFFFFF, 0x8F00FF, 1);
  }

  call_free(processList);
  while (1) {
    int letter = call_getChar();
    if (letter != 0) {
      call_exit();
    }
  }
}