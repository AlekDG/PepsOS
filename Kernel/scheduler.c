/**
 * Fido-Dido Scheduler. A simple round-robin scheduler for PepsOS.
*/
#include <stdint.h>
#include <stdio.h>
#include "include/lib.h"
#include "include/memMan.h"

extern uint64_t prepareStack(uint64_t rsp, uint64_t rip);

typedef int (*newProcess) (int,char*);

static int nextPid = 0;

typedef enum State{
    BLOCKED,
    READY,
    RUNNING,
} State;

typedef struct {
    regStruct registros;   //Preguntar si esta bien, porque esto ya lo tenemos en el stack.
    uint64_t rsp;                           //Esto si lo quiero porque tenemos que guardar el puntero al stack para retomar y poder hacer popState
    unsigned int pid;
    State state;
    int priority;                          //deberiamos ver si directamente implementamos queue entonces esta info no dbeeria estar aca
    Process *next;
}  Process; //podriamos ver de agregar el tiempo de quantum que corrio por si esta corriendo porque volvio de blocked y no por el timer tick

typedef struct processTable{        
    int processCount;
    Process *running;
    Process *ready;
    Process *lastReady;
    Process *blocked;
} processTable;
/*
la idea es tener una lista de procesos ready. B -> C -> D donde runningProcess = A y cuando se cumple
el quantum A pasa al final de la lista entonces queda running = B y lastProc = A quedando la lista
D -> C -> A (osea aplicamos round robin). Cuando un proc se bloquea pasa a la lista de bloqueados, al desbloquear
pasa o al final o al principio de ready y se llama al scheduler
*/

/**
 * Create a new process table
 * @return a new process table 
*/
processTable createPCB(){
    processTable pcb;
    pcb.processCount = 0;
    pcb.running = NULL;
    pcb.ready = NULL;
    pcb.lastReady = NULL;
    pcb.blocked = NULL;
    return pcb;
}

/**
 * Round Robin Scheduler implementation. Change the current running process for a ready one if it exists.
 * If the running process was blocked, it should be noted en the pcb table before calling this procedure.
 * @param pcb the process table
 * @param rsp the stack pointer of the current running process
 * @return the new rsp
*/
uint64_t scheduler(processTable pcb,uint64_t rsp){
    if(pcb.ready == NULL ){
        if(pcb.running->state != BLOCKED){
            //Si no hay nadie mas en estado ready, sigo con este proceso
            return rsp; 
            }
        //como no hay nadie ready, paso pcb.running a la lista de blocked y seteo como running a proceso que hace halt
        pcb.running->next = pcb.blocked;
        pcb.blocked = pcb.running;
        pcb.running = halt(); //VER
        return 0x00000000; //PONER STACK HALT!!
    }

   Process* current = pcb.running;
   current->rsp = rsp;

   if(pcb.running->state == BLOCKED){
     //paso a blocked y tomo el primero de ready como running 
     pcb.running->next = pcb.blocked;
     pcb.blocked = pcb.running;
     pcb.running = pcb.ready;
     pcb.ready = pcb.ready->next;
     pcb.running->next = NULL;
   }else{
     //si estoy aca el proceso running cumplio su quantum -> lo ponemos al final de la lista de ready 
     //y tomamos el primer ready como running 
     current->next = NULL;
     pcb.lastReady->next = current;
     pcb.lastReady = current;
     pcb.running = pcb.ready;
     pcb.ready = pcb.running->next;
     }

   return pcb.running->rsp;
   }


/**
 * Get current running process pid
 * @param pcb the proccess table
 * @return the pid of the running process
*/
   int getPid(processTable pcb){
    return pcb.running->pid;
   }


//ret 1 si lo bloqueo, 0 si no
int block(int pid, processTable pcb){
    //busco el proceso en la lista de ready y lo paso a blocked
    //si el proceso no esta en ready, no hago nada
}


//ret 1 si lo paso a ready, 0 si no
int unblock(int pid, processTable pcb){
    //busco el proceso en la lista de block y lo paso a ready
    //si el proceso no esta en block, no hago nada
}


int createProcess(newProcess process,int argc, char* argv, processTable pcb){
    uint64_t newProcessStack = 0;//= allocMemory(....);
    newProcessStack = prepareStack(newProcessStack,process);
    Process* newProcess = 0;//allocMemory(process);
    newProcess->pid = nextPid++;
    newProcess->priority = 0;
    newProcess->state = READY;
    newProcess->rsp = newProcessStack /*+ lo que agregue en prepareStack*/ ;
    newProcess->next = NULL;
    if(pcb.ready == NULL){
        pcb.ready = newProcess;
        pcb.lastReady = newProcess;
    }else{
        pcb.lastReady->next = newProcess;
    }
    return newProcess->pid;
}
