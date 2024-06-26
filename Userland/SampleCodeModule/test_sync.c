// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <testSyscall.h>
#include <test_util.h>
#include <UserSyscalls.h>

#include <user_lib.h>

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global;

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  my_yield();
  aux += inc;
  *p = aux;
}

void my_process_inc(uint64_t argc, char *argv[]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3){
      call_drawStringFormatted("Wrong number of arguments\n", WHITE, LIGHT_GRAY, 2);
      call_sleep(40);
      call_exit();
  }


  if ((n = satoi(argv[0])) <= 0){
      call_drawStringFormatted("Wrong argument 0\n", WHITE, LIGHT_GRAY, 2);
      call_sleep(40);
      call_exit();
  }

  if ((inc = satoi(argv[1])) == 0){
      call_drawStringFormatted("Wrong argument 1\n", WHITE, LIGHT_GRAY, 2);
      call_sleep(40);
      call_exit();
  }

  if ((use_sem = satoi(argv[2])) < 0){
      call_drawStringFormatted("Wrong argument 2\n", WHITE, LIGHT_GRAY, 2);
      call_sleep(40);
      call_exit();
  }


  if (use_sem)
    if (!my_sem_open(SEM_ID, 1)) {
      call_drawStringFormatted("test_sync: ERROR opening semaphore\n", RED, BLACK, 2);
        call_sleep(40);
      call_exit();
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      my_sem_wait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      my_sem_post(SEM_ID);
  }

  call_exit();

}

void test_sync(uint64_t argc, char *argv[]) {

  if (argc != 2)
    call_exit();

  char *argvDec[] = {argv[1], "-1", argv[2], NULL};
  char *argvInc[] = {argv[1], "1", argv[2], NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    my_create_process(my_process_inc, 3, argvDec);
   my_create_process(my_process_inc, 3, argvInc);
  }

  while(my_wait());

  my_sem_close(SEM_ID);

  call_drawStringFormatted("Final value: ", WHITE, BLACK, 2);
  call_printIntFormatted(global, WHITE, BLACK, 2);
  call_drawLetterFromChar('\n');

  call_sleep(40);
  call_exit();
}
