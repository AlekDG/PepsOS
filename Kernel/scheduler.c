// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**
 * Fido-Dido Scheduler. A simple round-robin scheduler for PepsOS.
 */
#include <lib.h>
#include <memMan.h>
#include <pipes.h>
#include <scheduler.h>
#include <stdint.h>
#include <time.h>

static int nextPid = 0;

static processTable pcb;


Process *createProcessStruct(newProcess process, int argc, char *argv[]);

void setPriorityQueuesNull() {
  for (int i = 0; i < MAX_PRIORITY; i++) {
    pcb.priorityQueue[i].lastReady = NULL;
    pcb.priorityQueue[i].ready = NULL;
  }
}

processTable *createPCB(void) {
  pcb.processCount = 0;
  pcb.running = NULL;
  setPriorityQueuesNull();
  pcb.blocked = NULL;
  char *haltArgv[] = {"halt"};
  pcb.halt = createProcessStruct(haltCpu, 0, haltArgv);
  setSleeped();
  return &pcb;
}

int decrementKidsCountOnBlockedProcess(int pid) {
  Process *aux = pcb.blocked;
  while (aux != NULL) {
    if (aux->pid == pid) {
      aux->kidsCount--;
      return 1;
    }
    aux = aux->next;
  }
  return 0;
}

int decrementKidsCountOnReadyProcess(int pid) {
  for (int i = 0; i < MAX_PRIORITY; i++) {
    Process *aux = pcb.priorityQueue[i].ready;
    while (aux != NULL) {
      if (aux->pid == pid) {
        aux->kidsCount--;
        return 1;
      }
      aux = aux->next;
    }
  }
  return 0;
}

void decrementKidsCount(int pid) {
  int wasDecremented = decrementKidsCountOnBlockedProcess(pid);
  if (!wasDecremented) {
    decrementKidsCountOnReadyProcess(pid);
  }
}

/**
 * Round Robin Scheduler implementation. Change the current running process for
 * a ready one if it exists. If the running process was blocked, it should be
 * noted en the pcb table before calling this procedure.
 * @param pcb the process table
 * @param rsp the stack pointer of the current running process
 * @return the new rsp
 */

/**
 * Get current running process pid
 * @param pcb the proccess table
 * @return the pid of the running process
 */
int getPid() { return pcb.running->pid; }

int block(int pid) {
  if (pcb.running->pid == pid) {
    pcb.running->state = BLOCKED;
    fireTimerInt();
    return 1;
  }

  for (int i = 0; i < MAX_PRIORITY; i++) {
    Process *aux = pcb.priorityQueue[i].ready;
    if (aux != NULL && aux->pid == pid) {
      aux->state = BLOCKED;
      pcb.priorityQueue[i].ready = aux->next;
      if (pcb.priorityQueue[i].ready == NULL)
        pcb.priorityQueue[i].lastReady = NULL;
      aux->next = pcb.blocked;
      pcb.blocked = aux;
      return 1;
    }
    while (aux != NULL) {
      if (aux->next != NULL && aux->next->pid == pid) {
        Process *blocked = aux->next;
        blocked->state = BLOCKED;
        aux->next = blocked->next;
        if (blocked == pcb.priorityQueue[i].lastReady) {
          pcb.priorityQueue[i].lastReady = aux;
        }
        blocked->next = pcb.blocked;
        pcb.blocked = blocked;
        return 1;
      }
      aux = aux->next;
    }
  }
  return 0;
}

int unblock(int pid) {
  Process *aux = pcb.blocked;
  if (aux != NULL && aux->pid == pid) {
    aux->state = READY;
    pcb.blocked = aux->next;
    if (pcb.priorityQueue[aux->priority].ready == NULL) {
      pcb.priorityQueue[aux->priority].ready = aux;
      pcb.priorityQueue[aux->priority].lastReady = aux;

    } else {
      pcb.priorityQueue[aux->priority].lastReady->next = aux;
      pcb.priorityQueue[aux->priority].lastReady = aux;
    }
    aux->next = NULL;
    return 1;
  }
  while (aux != NULL) {
    if (aux->next != NULL && aux->next->pid == pid) {
      Process *unblocked = aux->next;
      unblocked->state = READY;
      aux->next = unblocked->next;
      if (pcb.priorityQueue[unblocked->priority].ready == NULL) {
        pcb.priorityQueue[unblocked->priority].ready = unblocked;
        pcb.priorityQueue[unblocked->priority].lastReady = unblocked;

      } else {
        pcb.priorityQueue[unblocked->priority].lastReady->next = unblocked;
        pcb.priorityQueue[unblocked->priority].lastReady = unblocked;
      }
      unblocked->next = NULL;
      return 1;
    }
    aux = aux->next;
  }
  return 0;
}

void copyArgvOnStack(char *rsp, int argc, char *argv[]) {
  int charsCopyCount = 0;
  char **newArgv = (void *)rsp;
  rsp += sizeof(char *) * (argc + 1);
  for (int i = 0; i <= argc; i++) {
    int j = 0;
    newArgv[i] = rsp + charsCopyCount;
    while (argv[i][j] != (uint64_t) NULL) {
      rsp[charsCopyCount++] = argv[i][j];
      j++;
    }
    rsp[charsCopyCount++] = '\0';
  }
}

Process *createProcessStruct(newProcess process, int argc, char *argv[]) {
  int s = PROCESS_STACK_SIZE;
  void *startAdress = allocMemory(s);
  copyArgvOnStack((char *)startAdress, argc, argv);
  void *newProcessStack =
      startAdress +
      PROCESS_STACK_SIZE;
  newProcessStack =
      prepareStack(newProcessStack, (uint64_t)process, argc, startAdress);
  Process *newProcess = allocMemory(sizeof(Process));
  newProcess->pid = nextPid++;
  newProcess->priority = 0;
  newProcess->state = READY;
  newProcess->rsp = newProcessStack;
  newProcess->rbp = newProcessStack;
  newProcess->next = NULL;
  newProcess->memStartAdress = startAdress;
  newProcess->name = argv[0];
  newProcess->kidsCount = 0;
  return newProcess;
}

int createProcess(newProcess process, int argc, char *argv[], int priority,
                  processType tipo, int *rw_pipes) {
  Process *newProcess;
  if (0 <= priority && priority < MAX_PRIORITY) {
    newProcess = createProcessStruct(
        process, argc, argv);
    newProcess->tipo =
        tipo;
    pcb.processCount++;
    newProcess->priority = priority;
    if (rw_pipes == 0) {
      newProcess->in_pipe = STDIN_PIPE;
      newProcess->out_pipe = STDOUT_PIPE;
    } else {
      newProcess->in_pipe = rw_pipes[0];
      newProcess->out_pipe = rw_pipes[1];
    }
  } else {
    return -1;
  }

  if (pcb.running != NULL) {
    newProcess->parentPID = pcb.running->pid;
    pcb.running->kidsCount++;
  } else {
    newProcess->parentPID = 0;
  }
  if (pcb.priorityQueue[priority].ready == NULL) {
    pcb.priorityQueue[priority].ready = newProcess;
    pcb.priorityQueue[priority].lastReady = newProcess;
  } else {
    pcb.priorityQueue[priority].lastReady->next = newProcess;
    pcb.priorityQueue[priority].lastReady = newProcess;
  }
  return newProcess->pid;
}

int killBlocked(int pid, int *parent) {
  Process *aux = pcb.blocked;
  if (aux != NULL && aux->pid == pid) {
    pcb.blocked = aux->next;
    *parent = aux->parentPID;
    freeMemory(aux->memStartAdress);
    freeMemory(aux);
    return 1;
  }
  while (aux != NULL) {
    if (aux->next != NULL && aux->next->pid == pid) {
      Process *toKill = aux->next;
      aux->next = toKill->next;
      *parent = toKill->parentPID;
      freeMemory(toKill->memStartAdress);
      freeMemory(toKill);
      return 1;
    }
    aux = aux->next;
  }
  return 0;
}

int killReady(int pid, int *parent) {
  for (int i = 0; i < MAX_PRIORITY; i++) {
    Process *aux = pcb.priorityQueue[i].ready;
    if (aux != NULL && aux->pid == pid) {
      pcb.priorityQueue[i].ready = aux->next;
      if (pcb.priorityQueue[i].ready == NULL)
        pcb.priorityQueue[i].lastReady = NULL;
      *parent = aux->parentPID;
      freeMemory(aux->memStartAdress);
      freeMemory(aux);
      return 1;
    }
    while (aux != NULL) {
      if (aux->next != NULL && aux->next->pid == pid) {
        Process *toKill = aux->next;
        aux->next = toKill->next;
        if (toKill == pcb.priorityQueue[i].lastReady) {
          pcb.priorityQueue[i].lastReady = aux;
        }
        *parent = toKill->parentPID;
        freeMemory(toKill->memStartAdress);
        freeMemory(toKill);
        return 1;
      }
      aux = aux->next;
    }
  }
  return 0;
}

int kill(int pid) {
  int parentPid;
  if (pcb.running->pid == pid) {
    pcb.running->state = EXITED;
    unblock(pcb.running->parentPID);
    fireTimerInt();
  }
  int wasKilled = killBlocked(pid, &parentPid);
  if (!wasKilled) {
    wasKilled = killReady(pid, &parentPid);
  }
  if (wasKilled) {
    pcb.processCount--;
    decrementKidsCount(parentPid);
    unblock(parentPid);
  }
  return wasKilled;
}

void startFirstProcess() {
  pcb.running = pcb.priorityQueue[0].ready;
  pcb.priorityQueue[0].ready = pcb.priorityQueue[0].lastReady = NULL;
  setFirstProcess(pcb.running->rsp);
}

int createBackgroundProcess(newProcess process, int argc, char *argv[],
                            int priority, int *rw_pipes) {
  return createProcess(process, argc, argv, priority, BACKGROUND, rw_pipes);
}

int createForegroundProcess(newProcess process, int argc, char *argv[],
                            int priority, int *rw_pipes) {
  int pid = createProcess(process, argc, argv, priority, FOREGROUND, rw_pipes);
  block(pcb.running->pid);
  return pid;
}

int wait() {
  if (pcb.running->kidsCount == 0) {
    return 0;
  }
  block(pcb.running->pid);
  return 1;
}

void exit() {
  pcb.running->state = EXITED;
  unblock(pcb.running->parentPID);
  decrementKidsCount(pcb.running->parentPID);
  fireTimerInt();
}


void yield() { fireTimerInt(); }

void *priorityScheduler(void *rsp, void *rbp) {
  Process *running = pcb.running;
  running->rbp = rbp;
  running->rsp = rsp;
  if (running->state == EXITED) {
    freeMemory(running->memStartAdress);
    freeMemory(running);
    pcb.processCount--;
    pcb.running = pcb.halt;
  } else if (running->state == BLOCKED) {
    pcb.running->next = pcb.blocked;
    pcb.blocked = pcb.running;
    pcb.running = pcb.halt;
  }

  if (pcb.running->priority > 0)
    pcb.running->priority--;

  for (int i = MAX_PRIORITY - 1; 0 <= i; i--) {

    if (pcb.priorityQueue[i].ready == NULL) {
      if (pcb.running->priority == i) {
        return pcb.running->rsp;
      }
      continue;
    }

    if (pcb.running ==
        pcb.halt) {
      pcb.running = pcb.priorityQueue[i].ready;
      pcb.priorityQueue[i].ready = pcb.priorityQueue[i].ready->next;
      if (pcb.priorityQueue[i].ready == NULL) {
        pcb.priorityQueue[i].lastReady = NULL;
      }

    } else {
      pcb.running->next = NULL;
      pcb.running->state = READY;
      if (pcb.priorityQueue[pcb.running->priority].ready == NULL) {
        pcb.priorityQueue[pcb.running->priority].ready = pcb.running;
      }
      pcb.priorityQueue[pcb.running->priority].lastReady->next = running;
      pcb.priorityQueue[pcb.running->priority].lastReady = running;

      pcb.running = pcb.priorityQueue[i].ready;
      pcb.priorityQueue[i].ready = pcb.running->next;
      if (pcb.priorityQueue[i].ready == NULL) {
        pcb.priorityQueue[i].lastReady = NULL;
      }
    }
    break;
  }
  pcb.running->state = RUNNING;
  return pcb.running->rsp;
}

int changeBlockedProcessPriority(int pid, int priority) {
  Process *aux = pcb.blocked;
  while (aux != NULL) {
    if (aux->pid == pid) {
      aux->priority = priority;
      return 1;
    }
    aux = aux->next;
  }
  return 0;
}

int changeReadyProcessPriority(int pid, int priority) {
  for (int i = 0; i < MAX_PRIORITY; i++) {
    Process *aux = pcb.priorityQueue[i].ready;
    if (aux != NULL && aux->pid == pid) {
      pcb.priorityQueue[i].ready = aux->next;
      if (pcb.priorityQueue[i].ready == NULL) {
        pcb.priorityQueue[i].lastReady = NULL;
      }

      if (pcb.priorityQueue[priority].ready == NULL) {
        pcb.priorityQueue[priority].ready = aux;
        pcb.priorityQueue[priority].lastReady = aux;
      } else {
        pcb.priorityQueue[priority].lastReady->next = aux;
        pcb.priorityQueue[priority].lastReady = aux;
      }
      aux->next = NULL;
      return 1;
    }
    while (aux != NULL) {
      if (aux->next != NULL && aux->next->pid == pid) {
        Process *toChange = aux->next;
        toChange->priority = priority;
        aux->next = toChange->next;
        if (toChange == pcb.priorityQueue[i].lastReady) {
          pcb.priorityQueue[i].lastReady = aux;
        }

        if (pcb.priorityQueue[priority].ready == NULL) {
          pcb.priorityQueue[priority].ready = toChange;
          pcb.priorityQueue[priority].lastReady = toChange;
        } else {
          pcb.priorityQueue[priority].lastReady->next = toChange;
          pcb.priorityQueue[priority].lastReady = toChange;
        }
        toChange->next = NULL;
        return 1;
      }
      aux = aux->next;
    }
  }
  return 0;
}

int changePriority(int pid, int priority) {
  if (priority < 0 || priority >= MAX_PRIORITY) {
    return 0;
  }
  if (pcb.running->pid == pid) {
    pcb.running->priority = priority;
    return 1;
  }
  int wasChanged = changeBlockedProcessPriority(pid, priority);
  if (!wasChanged) {
    wasChanged = changeReadyProcessPriority(pid, priority);
  }
  return wasChanged;
}

void copyProcToArray(int indexCount, processInfo *procs, Process *process) {
  procs[indexCount].name = process->name;
  procs[indexCount].parentPid = process->parentPID;
  procs[indexCount].pid = process->pid;
  procs[indexCount].priority = process->priority;
  procs[indexCount].rbp = process->rbp;
  procs[indexCount].rsp = process->rsp;
  procs[indexCount].state = process->state;
  procs[indexCount].tipo = process->tipo;
}

void copyListOnArrayFromIndex(int *indexCount, processInfo *procs,
                              Process *list) {
  Process *process = list;
  while (process != NULL) {
    copyProcToArray(*indexCount, procs, process);
    (*indexCount)++;
    process = process->next;
  }
}


processInfo *getAllProcessInfo(int *count) {
  processInfo *procs = allocMemory(pcb.processCount * sizeof(processInfo));
  int indexCount = 0;

  copyListOnArrayFromIndex(&indexCount, procs, pcb.blocked);

  for (int i = 0; i < MAX_PRIORITY; i++) {
    copyListOnArrayFromIndex(&indexCount, procs, pcb.priorityQueue[i].ready);
  }

  copyProcToArray(indexCount++, procs, pcb.running);
  *count = indexCount;
  return procs;
}

void sleep(int numberOfTicks) {
  if (numberOfTicks > 0) {
    addProcSleep(pcb.running->pid, numberOfTicks);
    block(pcb.running->pid);
  }
}

int *getRunningProcessStdin() {
  int* toRet = allocMemory(2);
  toRet[0] = pcb.running->in_pipe;
  toRet[1] = pcb.running->out_pipe;
  return toRet;
}

Process *get_current_proc() { return pcb.running; }

int currentProcType() { return pcb.running->tipo == FOREGROUND; }

void createProcessWithPipes(newProcess processes1, newProcess processes2, int *argcs, char **argvs[],
                            int **pipes) {
  createProcess(processes1, argcs[0], argvs[0], 0, FOREGROUND, pipes[0]);
  createProcess(processes2, argcs[1], argvs[1], 0, FOREGROUND, pipes[1]);
  block(pcb.running->pid);
}