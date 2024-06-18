// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <UserSyscalls.h>

#define START_POSITION 50
#define PRINT_INFO_START_POSITION 60
#define FONT_SIZE 2
#define PEPSIBLUE 0x0000FF
#define BLACK 0x000000
#define BLUE 0x0000AA
#define GREEN 0x00AA00
#define CYAN 0x00AAAA
#define MAGENTA 0xAA00AA
#define BROWN 0xAA5500
#define LIGHT_GRAY 0xAAAAAA
#define DARK_GRAY 0x555555
#define BRIGHT_BLUE 0x5555FF
#define BRIGHT_GREEN 0x55FF55
#define BRIGHT_CYAN 0x55FFFF
#define BRIGHT_RED 0xFF5555
#define BRIGHT_MAGENTA 0xFF55FF
#define YELLOW 0xFFFF55
#define WHITE 0xFFFFFF

void ps() {
  int processCount;
  processInfo *processList = call_ps(&processCount);
  call_paintScreen(0x0);
  call_setXBuffer(START_POSITION);
  call_setYBuffer(START_POSITION);
  call_setSize(FONT_SIZE);
  call_set_color(WHITE,BLACK);

  call_drawStringFormatted("PS for PepsOS - 2024 All Rights Not Reserved",
                           WHITE, PEPSIBLUE, FONT_SIZE);
  call_drawStringFormatted("\n", WHITE, PEPSIBLUE, FONT_SIZE);
  call_drawLetterFromChar('\n');
  call_drawStringFormatted(
      "NAME - PID - PARENT PID - PRIORITY - STATE - TYPE - RSP - RBP", WHITE,
      DARK_GRAY, FONT_SIZE);
  call_setXBuffer(PRINT_INFO_START_POSITION);
  for (int i = 0; i < processCount; i++) {
    call_printProcessInfo(&processList[i]);
  }

  call_free(processList);
  call_pipe_read(STDIN);
  call_set_color(DARK_GRAY,LIGHT_GRAY);
  call_exit();
}