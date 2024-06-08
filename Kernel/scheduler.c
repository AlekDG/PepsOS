/**
 * Fido-Dido Scheduler. A simple round-robin scheduler for PepsOS.
 */
#include <BlockMemoryManager.h>
#include <lib.h>
#include <memMan.h>
#include <scheduler.h>
#include <stdint.h>
#include <pipes.h>

static int nextPid = 0;

static processTable pcb;

/*
la idea es tener una lista de procesos ready. B -> C -> D donde runningProcess =
A y cuando se cumple el quantum A pasa al final de la lista entonces queda
running = B y lastProc = A quedando la lista D -> C -> A (osea aplicamos round
robin). Cuando un proc se bloquea pasa a la lista de bloqueados, al desbloquear
pasa o al final o al principio de ready y se llama al scheduler
*/

Process *createProcessStruct(newProcess process, int argc, char *argv);

void setPriorityQueuesNull() {
  for (int i = 0; i < MAX_PRIORITY; i++) {
    pcb.priorityQueue[i].lastReady = NULL;
    pcb.priorityQueue[i].ready = NULL;
  }
}

/**
 * Create a new process table
 * @return a new process table
 */
processTable *createPCB(void) {
  pcb.processCount = 0;
  pcb.running = NULL;
  setPriorityQueuesNull();
  pcb.blocked = NULL;
  pcb.halt = createProcessStruct(haltCpu, 0, "halt");
  return &pcb;
}

/**
 * Round Robin Scheduler implementation. Change the current running process for
 * a ready one if it exists. If the running process was blocked, it should be
 * noted en the pcb table before calling this procedure.
 * @param pcb the process table
 * @param rsp the stack pointer of the current running process
 * @return the new rsp
 */
void *scheduler(void *rsp) {
  pcb.running->rsp = rsp;
  if (pcb.ready == NULL) {
    if (pcb.running->state != BLOCKED && pcb.running->state != EXITED) {
      // Si no hay nadie mas en estado ready, sigo con este proceso
      return rsp;
    }
    // como no hay nadie ready, paso pcb.running a la lista de blocked y seteo
    // como running a proceso que hace halt
    if (pcb.running->state == EXITED) {
      // hago free
      pcb.running = NULL;
      pcb.running = pcb.halt;
    } else if (pcb.running != pcb.halt) {
      pcb.running->next = pcb.blocked;
      pcb.blocked = pcb.running;
      pcb.running = pcb.halt;
    }
    return pcb.running->rsp;
  }

  if (pcb.running == pcb.halt) {
    pcb.running = pcb.ready;
    pcb.ready = pcb.running->next; // y actualizar last ready
    if (pcb.ready == NULL)
      pcb.lastReady = NULL; // no hay readys
  } else {

    Process *current = pcb.running;
    if (pcb.running->state == EXITED) {
      pcb.running = pcb.ready;
      pcb.ready = pcb.running->next;
      if (pcb.ready == NULL)
        pcb.lastReady = NULL;
      // METER free
      return pcb.running->rsp;
    }
    if (pcb.running->state == BLOCKED) {
      // paso a blocked y tomo el primero de ready como running
      pcb.running->next = pcb.blocked;
      pcb.blocked = pcb.running;
      pcb.running = pcb.ready;
      pcb.ready = pcb.ready->next;
      if (pcb.ready == NULL)
        pcb.lastReady = NULL;
      pcb.running->next = NULL;
    } else {
      // si estoy aca el proceso running cumplio su quantum -> lo ponemos al
      // final de la lista de ready y tomamos el primer ready como running
      current->next = NULL;
      pcb.lastReady->next = current;
      pcb.lastReady = current;
      pcb.running = pcb.ready;
      pcb.ready = pcb.running->next;
    }
  }
  return pcb.running->rsp;
}

/**
 * Get current running process pid
 * @param pcb the proccess table
 * @return the pid of the running process
 */
int getPid() { return pcb.running->pid; }

// ret 1 si lo bloqueo, 0 si no
int block(int pid) {
  // busco el proceso en las listas de ready y lo paso a blocked
  // si el proceso no esta en ready, no hago nada
  if (pcb.running->pid == pid) {
    pcb.running->state = BLOCKED;
    // Interrupt 0x20 para hacer el cambio de contexto
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

// ret 1 si lo paso a ready, 0 si no
int unblock(int pid) {
  // busco el proceso en la lista de block y lo paso a ready
  // si el proceso no esta en block, no hago nada
  Process *aux = pcb.blocked;
  if (aux != NULL && aux->pid == pid) {
    aux->state = READY;
    pcb.blocked = aux->next;
    if (pcb.priorityQueue[aux->priority].ready == NULL) {
      pcb.priorityQueue[aux->priority].ready = aux;
      pcb.priorityQueue[aux->priority].lastReady = aux;

    } else {
      pcb.lastReady->next = aux;
      pcb.lastReady = aux;
    }
    aux->next = NULL;
    return 1;
  }
  while (aux != NULL) {
    if (aux->next != NULL && aux->next->pid == pid) {
      Process *unblocked = aux->next;
      unblocked->state = READY;
      aux->next = unblocked->next;
      if (pcb.priorityQueue[aux->priority].ready == NULL) {
        pcb.priorityQueue[aux->priority].ready = unblocked;
        pcb.priorityQueue[aux->priority].lastReady = unblocked;

      } else {
        pcb.lastReady->next = unblocked;
        pcb.lastReady = unblocked;
      }
      unblocked->next = NULL;
      return 1;
    }
    aux = aux->next;
  }
  return 0;
}

Process *createProcessStruct(newProcess process, int argc, char *argv) {
  int s = PROCESS_STACK_SIZE;
  void *startAdress = allocMemory(s);
  void *newProcessStack =
      startAdress +
      PROCESS_STACK_SIZE; // VER DE AGREGAR VERIFICACION ret == NULL
  newProcessStack =
      prepareStack(newProcessStack, (uint64_t)process, 0x876, 0x678);
  Process *newProcess = allocMemory(sizeof(Process)); // ACA SAME
  newProcess->pid = nextPid++;
  newProcess->priority = 0;
  newProcess->state = READY;
  newProcess->rsp = newProcessStack /*+ lo que agregue en prepareStack*/;
  newProcess->next = NULL;
  newProcess->memStartAdress = startAdress;
  return newProcess;
}

int createProcess(newProcess process, int argc, char *argv, int priority) {
  Process *newProcess;
  if (0 <= priority && priority < MAX_PRIORITY) {
    newProcess = createProcessStruct(
        process, argc, argv); // AGREGAR CHECK si es null hay error
    newProcess->tipo =
        FOREGROUND; // AGREGAR COMO ARGUMENTO, ESTO ES PARA DEBUG RAPIDO
    pcb.processCount++;
    newProcess->priority = priority;
  } else {
    return -1; // ERROR
  }

  if (pcb.running != NULL) {
    newProcess->parentPID = pcb.running->pid;
  } else {
    // es el primer proceso
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

int killBlocked(int pid) {
  Process *aux = pcb.blocked;
  if (aux != NULL && aux->pid == pid) {
    pcb.blocked = aux->next;
    freeMemory(aux->memStartAdress);
    freeMemory(aux);
    return 1;
  }
  while (aux != NULL) {
    if (aux->next != NULL && aux->next->pid == pid) {
      Process *toKill = aux->next;
      aux->next = toKill->next;
      freeMemory(toKill->memStartAdress);
      freeMemory(toKill);
      return 1;
    }
    aux = aux->next;
  }
  return 0;
}

int killReady(int pid) {
  for (int i = 0; i < MAX_PRIORITY; i++) {
    Process *aux = pcb.priorityQueue[i].ready;
    if (aux != NULL && aux->pid == pid) {
      pcb.priorityQueue[i].ready = aux->next;
      if (pcb.priorityQueue[i].ready == NULL)
        pcb.priorityQueue[i].lastReady = NULL;
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
        freeMemory(toKill->memStartAdress);
        freeMemory(toKill);
        return 1;
      }
      aux = aux->next;
    }
  }
  return 0;
}
// retorna 1 si lo mato, 0 si no
int kill(int pid) {
  if (pcb.running->pid == pid) {
    pcb.running->state = EXITED;
    fireTimerInt();
  }
  int wasKilled = killBlocked(pid);
  if (!wasKilled) {
    wasKilled = killReady(pid);
  }
  return wasKilled;
}

void startFirstProcess() {
  pcb.running = pcb.priorityQueue[0].ready;
  pcb.priorityQueue[0].ready = pcb.priorityQueue[0].lastReady = NULL;
  setFirstProcess(pcb.running->rsp);
}

int createBackgroundProcess(newProcess process, int argc, char *argv,
                            int priority) {
  // no bloqueo el actual
  return createProcess(process, argc, argv, priority);
}

int createForegroundProcess(newProcess process, int argc, char *argv,
                            int priority) {
  // bloqueo el actual
  int pid = createProcess(process, argc, argv, priority);
  block(pcb.running->pid);
  return pid;
}

void exit() {
  pcb.running->state = EXITED;
  if (pcb.running->tipo == FOREGROUND) { // desbloquo al padre!
    unblock(pcb.running->parentPID);
  }
  pcb.processCount--;
  fireTimerInt();
}

/**
 * Renunciar al cpu
 * Si no hay procesos libres se asegura que seguira ejecutando el proceso
 * llamador
 */
void yield() { fireTimerInt(); }

/// @brief
/// @param rsp
/// @return
void *priorityScheduler(void *rsp) {
  Process *running = pcb.running;
  running->rsp = rsp;
  if (running->state == EXITED) {
    freeMemory(running->memStartAdress);
    freeMemory(running);
    pcb.running = pcb.halt; // si encuentro algun prceso ready despues lo cambio
  } else if (running->state == BLOCKED) {
    pcb.running->next = pcb.blocked;
    pcb.blocked = pcb.running;
    pcb.running = pcb.halt;
  }

  // el proceso cumplio su quantum
  if (pcb.running->priority > 0)
    pcb.running->priority--;

  // busco el primer ready con mayor prioridad para hacer el switch
  for (int i = MAX_PRIORITY - 1; 0 <= i; i--) {
    // Process* ready = pcb.priorityQueue[i].ready;
    // Process* lastReady = pcb.priorityQueue[i].lastReady;
    if (pcb.priorityQueue[i].ready == NULL) {
      // No hay procs ready con prioridad i
      if (pcb.running->priority == i) {
        return pcb.running->rsp; // Le doy un quantum mas
      }
      continue; // busco en prioridad i-1
    }

    if (pcb.running ==
        pcb.halt) { // si es halt,no quiero ponerlo en la lista de ready
      pcb.running = pcb.priorityQueue[i].ready;
      pcb.priorityQueue[i].ready = pcb.priorityQueue[i].ready->next;
      if (pcb.priorityQueue[i].ready == NULL)
        pcb.priorityQueue[i].lastReady = NULL;
    } else {
      pcb.running->next = NULL;
      pcb.priorityQueue[i].lastReady->next = running;
      pcb.priorityQueue[i].lastReady = running;
      pcb.running = pcb.priorityQueue[i].ready;
      pcb.priorityQueue[i].ready = pcb.running->next;
    }
    break;
  }
  return pcb.running->rsp;
}



int changeBlockedProcessPriority(int pid, int priority){
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

int changeReadyProcessPriority(int pid, int priority){
  for (int i = 0; i < MAX_PRIORITY; i++) {
    Process *aux = pcb.priorityQueue[i].ready;
    while (aux != NULL) {
      if (aux->pid == pid) {
        aux->priority = priority;
        return 1;
      }
      aux = aux->next;
    }
  }
  return 0;
}

/**
 * Change the priority of a process
 * @param pid the process id
 * @param priority the new priority
 * @return 1 if the priority was changed, 0 if not
*/
int changePriority(int pid, int priority){
  if(priority < 0 || priority >= MAX_PRIORITY){
    return 0;
  }
  if(pcb.running->pid == pid){
     pcb.running->priority = priority;
     return 1;
  }
  int wasChanged = changeBlockedProcessPriority(pid,priority);
  if(!wasChanged){
    wasChanged = changeReadyProcessPriority(pid,priority);
  }
  return wasChanged;
}

typedef struct processInfo{
  char* name;
  void *rsp;
  void *rbp;
  unsigned int pid;
  unsigned int parentPid;
  unsigned int prioriy;
  State state;
  processType tipo;
  struct processInfo* next;
} processInfo;



void copyProcToArray(int* indexCount, processInfo* procs, Process* process){
    procs[*indexCount].name = process->name;
    procs[*indexCount].parentPid = process->parentPID;
    procs[*indexCount].pid = process->pid;
    procs[*indexCount].prioriy = process->priority;
    procs[*indexCount].rbp = process->rbp;
    procs[*indexCount].rsp = process->rsp;
    procs[*indexCount].state = process->state;
    procs[*indexCount].tipo = process->tipo;
}


void copyListOnArrayFromIndex(int* indexCount, processInfo* procs, Process* list){
  Process* process = list;
  while(process != NULL){
      copyProcToArray(indexCount,procs,process);
      (*indexCount)++;
      process = process->next;
  }  
}

/**
 * Makes an array with the information of each process on the system.
 * @return An array cointaning the information of the procceses null terminated.
 * Must be free by calee when its no longer needed
*/
processInfo* getAllProcessInfo(){
  processInfo* procs = allocMemory(pcb.processCount * sizeof(processInfo));
  int indexCount = 0;
  
  copyListOnArrayFromIndex(&indexCount,procs,pcb.blocked); 

  for(int i = 0; i < MAX_PRIORITY; i++){
    copyListOnArrayFromIndex(&indexCount,procs,pcb.priorityQueue[i].ready);
  }

  copyProcToArray(&indexCount,procs,pcb.running);

  return procs;
}