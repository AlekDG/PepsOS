// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <testSyscall.h>
#include <test_util.h>


enum States { RUNNINGt, BLOCKEDt, KILLEDt };

typedef struct P_rq {
  int32_t pid;
  enum States state;
} p_rq;

int64_t test_processes(int maxProcesses) {
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes = maxProcesses;
  char *argvAux[] = {"endless", "100"};

  p_rq p_rqs[max_processes];



    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      p_rqs[rq].pid = my_create_process(endless_loop_print, 1, argvAux);
      if (p_rqs[rq].pid == -1) {
        call_drawStringFormatted("test_processes: ERROR creating process\n",
                                     RED, BLACK, 2);
        return -1;
      } 
      p_rqs[rq].state = RUNNINGt;
      alive++;
    }
  

  // Randomly kills, blocks or unblocks processes until every one has been
  // killed
  while (alive > 0) {

    for (rq = 0; rq < max_processes; rq++) {
      action = GetUniform(100) % 2;

      switch (action) {
      case 0:
        if (p_rqs[rq].state == RUNNINGt || p_rqs[rq].state == BLOCKEDt) {
          if (my_kill(p_rqs[rq].pid) == 0) {
            call_drawStringFormatted("test_processes: ERROR killing process\n",
                                     RED, BLACK, 2);
            return -1;
          }
          p_rqs[rq].state = KILLEDt;
          alive--;
        }
        break;

      case 1:
        if (p_rqs[rq].state == RUNNINGt) {
          if (my_block(p_rqs[rq].pid) == 0) {
            call_drawStringFormatted("test_processes: ERROR blocking process\n",
                                     RED, BLACK, 2);
            return -1;
          }
          p_rqs[rq].state = BLOCKEDt;
        }
        break;
      }
    }

    // Randomly unblocks processes
    for (rq = 0; rq < max_processes; rq++) {
      if (p_rqs[rq].state == BLOCKEDt && GetUniform(100) % 2) {
        if (my_unblock(p_rqs[rq].pid) == 0) {
          call_drawStringFormatted("test_processes: ERROR unblocking process\n",
                                   WHITE, BLACK, 2);
          return -1;
        }
        p_rqs[rq].state = RUNNINGt;
      }
    }
  }
  return 1;
}
