#include <memMan.h>
#include <scheduler.h>
#include <time.h>

extern void _hlt();

static unsigned long ticks = 0;

typedef struct procSleep {
  int pid;
  int ticksLeft;
  struct procSleep *next;
} procSleep;

static procSleep *sleeped;

void setSleeped() { sleeped = NULL; }

void addProcSleep(int pid, int ticks) {
  procSleep *aux = sleeped;
  while (aux != NULL) {
    if (aux->pid == pid) {
      aux->ticksLeft += ticks;
      return;
    }
    aux = aux->next;
  }
  procSleep *newSleep = allocMemory(sizeof(procSleep));
  newSleep->pid = pid;
  newSleep->ticksLeft = ticks;
  newSleep->next = sleeped;
  sleeped = newSleep;
}

procSleep *checkTimers(procSleep *proc) {
  if (proc == NULL) {
    return proc;
  }
  proc->ticksLeft--;
  if (proc->ticksLeft == 0) {
    unblock(proc->pid);
    procSleep *aux = proc->next;
    freeMemory(proc);
    return aux;
  }
  proc->next = checkTimers(proc->next);
  return proc;
}

void timer_handler() {
  ticks++;
  sleeped = checkTimers(sleeped);
}

int ticks_elapsed() { return ticks; }

int seconds_elapsed() { return ticks / 18; }

void timer_wait(int time) {
  int startTime = ticks;
  while (ticks - startTime < time) {
    _hlt();
  }
}
