#include <stdint.h>
#include <UserSyscalls.h>

int64_t my_getpid() {
  return call_getPid();
}

int64_t my_create_process(void process(), uint64_t argc, char *argv[]) {
  return call_createBackgroundProcess(process,argc,argv,0);
}

int64_t my_nice(uint64_t pid, uint64_t newPrio) {
  return call_changePriority(pid,newPrio);
}

int64_t my_kill(uint64_t pid) {
  return call_kill(pid);
}

int64_t my_block(uint64_t pid) {
  return call_block(pid);
}

int64_t my_unblock(uint64_t pid) {
  return call_unblock(pid);
}

int64_t my_sem_open(char *sem_id, uint64_t initialValue) {
  return call_sem_open(initialValue,sem_id);
}

int64_t my_sem_wait(char *sem_id) {
  return call_sem_wait(call_get_sem_by_name(sem_id));
}

int64_t my_sem_post(char *sem_id) {
  return call_sem_post(call_get_sem_by_name(sem_id));
}

int64_t my_sem_close(char *sem_id) {
  return call_sem_close(sem_id);
}

int64_t my_yield() {
   call_yield();
   return 1;
}

int64_t my_wait(int64_t pid) {
  return call_waitKids();
}
