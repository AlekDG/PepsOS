// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <testSyscall.h>
#include <test_util.h>

#define MINOR_WAIT                                                             \
  1000000
#define WAIT                                                                   \
  20

#define TOTAL_PROCESSES 3
#define LOWEST 0  // TODO: Change as required
#define MEDIUM 2  // TODO: Change as required
#define HIGHEST 4 // TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio() {
  int64_t pids[TOTAL_PROCESSES];
  char *argv[] = {"endless", "10000000"};

  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = my_create_process(endless_loop_print, 1, argv);

  call_wait(WAIT);
  call_drawStringFormatted("\nCHANGING PRIORITIES...\n", WHITE, BLACK, 2);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], prio[i]);

  call_wait(WAIT);
  call_drawStringFormatted("\nBLOCKING...\n", WHITE, BLACK, 2);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_block(pids[i]);

  call_drawStringFormatted("CHANGING PRIORITIES WHILE BLOCKED...\n", WHITE,
                           BLACK, 2);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], MEDIUM);

  call_drawStringFormatted("UNBLOCKING...\n", WHITE, BLACK, 2);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_unblock(pids[i]);

  bussy_wait(WAIT);
  call_drawStringFormatted("\nKILLING...\n", WHITE, BLACK, 2);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_kill(pids[i]);
}
