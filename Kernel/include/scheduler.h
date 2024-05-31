#ifndef SCHEDULER_H
#define SCHEDULER_H

#define PROCESS_STACK_SIZE 50 
typedef int (*newProcess) (int,char*);

typedef enum State{
    BLOCKED,
    READY,
    RUNNING,
} State;

typedef struct process {
    regStruct registros;   //Preguntar si esta bien, porque esto ya lo tenemos en el stack.
    uint64_t rsp;                           //Esto si lo quiero porque tenemos que guardar el puntero al stack para retomar y poder hacer popState
    unsigned int pid;
    State state;
    int priority;                          //deberiamos ver si directamente implementamos queue entonces esta info no dbeeria estar aca
    struct process *next;
}  Process; //podriamos ver de agregar el tiempo de quantum que corrio por si esta corriendo porque volvio de blocked y no por el timer tick

typedef struct processTable{        
    int processCount;
    Process *running;
    Process *ready;
    Process *lastReady;
    Process *blocked;
    Process *halt;
} processTable;

uint64_t prepareStack(uint64_t rsp, uint64_t rip, int argc, char* argv);
int haltCpu();

/**
 * Create a new process table
 * @return a new process table 
*/
processTable* createPCB(MemoryManagerADT* memory);

/**
 * Round Robin Scheduler implementation. Change the current running process for a ready one if it exists.
 * If the running process was blocked, it should be noted en the pcb table before calling this procedure.
 * @param pcb the process table
 * @param rsp the stack pointer of the current running process
 * @return the new rsp
*/
uint64_t scheduler(uint64_t rsp);

/**
 * Get current running process pid
 * @param pcb the proccess table
 * @return the pid of the running process
*/
   int getPid();


   //ret 1 si lo bloqueo, 0 si no
int block(int pid);



//ret 1 si lo paso a ready, 0 si no
int unblock(int pid);

int createProcess(newProcess process,int argc, char* argv);

//retorna 1 si lo mato, 0 si no
int kill(int pid);

#endif