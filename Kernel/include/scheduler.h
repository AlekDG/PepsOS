#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../../Shared/include/ps.h"
#include <stdint.h>

#define PROCESS_STACK_SIZE 10000

typedef int (*newProcess)(int, char *);

typedef struct process {
  void *rsp;
  void *rbp;
  void *memStartAdress;
  unsigned int pid;
  State state;
  int priority;
  struct process *next;
  int parentPID;
  processType tipo;
  char *name;
  int kidsCount;
  int processStdin;
} Process;

#define MAX_PRIORITY 5

typedef struct readyList {
  Process *ready;
  Process *lastReady;
} readyList;

typedef struct processTable {
  int processCount;
  Process *running;

  readyList priorityQueue[MAX_PRIORITY];
  Process *blocked;
  Process *halt;
} processTable;

void *prepareStack(void *rsp, uint64_t rip, int argc, char *argv[]);
int haltCpu();

/**
 * Create a new process table
 * @return a new process table
 */
processTable *createPCB(void);

/**
 * Round Robin Scheduler implementation. Change the current running process for
 * a ready one if it exists. If the running process was blocked, it should be
 * noted en the pcb table before calling this procedure.
 * @param pcb the process table
 * @param rsp the stack pointer of the current running process
 * @return the new rsp
 */
void *scheduler(void *rsp);

/**
 * Get current running process pid
 * @param pcb the proccess table
 * @return the pid of the running process
 */
int getPid();

// ret 1 si lo bloqueo, 0 si no
int block(int pid);

// ret 1 si lo paso a ready, 0 si no
int unblock(int pid);

int createProcess(newProcess process, int argc, char *argv[], int priority,
                  processType tipo, int processStdin);

// retorna 1 si lo mato, 0 si no
int kill(int pid);

void startFirstProcess();

void setFirstProcess(void *rsp);

void fireTimerInt();

int createBackgroundProcess(newProcess process, int argc, char *argv[],
                            int priority, int processStdin);

int createForegroundProcess(newProcess process, int argc, char *argv[],
                            int priority, int processStdin);

void exit();

/**
 * Renunciar al cpu
 * Si no hay procesos libres se asegura que seguira ejecutando el proceso
 * llamador
 */
void yield();

/**
 * Change the priority of a process
 * @param pid the process id
 * @param priority the new priority
 * @return 1 if the priority was changed, 0 if not
 */
int changePriority(int pid, int priority);

/**
 * Makes an array with the information of each process on the system.
 * @return An array cointaning the information of the procceses null terminated.
 * Must be free by calee when its no longer needed
 */
processInfo *getAllProcessInfo(int *count);

void sleep(int numberOfTicks);

int wait();

int getRunningProcessStdin();

#endif