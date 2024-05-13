/**
 * Fido-Dido Scheduler. A simple round-robin scheduler for PepsOS.
*/
#include <stdint.h>
#include <stdio.h>
#include "include/lib.h"

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
 * Round Robin Scheduler implementation. Change the current running process for a ready one if it exists.
 * @param pcb the process table
 * @param rsp the stack pointer of the current running process
*/
void scheduler(processTable pcb,uint64_t rsp){
    if(pcb.ready != NULL ){
        if(pcb.running->state != BLOCKED){
            //Si no hay nadie mas en estado ready, sigo con este proceso
            return; }
        //paso pcb.running a blocked y seteo como running a proceso que hace halt
    }
   Process* current = pcb.running;
   current->rsp = rsp;

   if(pcb.running->state == BLOCKED){
     //paso a blocked y tomo el primero de ready como running 
   }
   //si estoy aca el proceso running cumplio su quantum -> lo ponemos al final de la lista de ready 
   //y tomamos el primer ready como running 
   pcb.lastReady->next = current;
   pcb.lastReady = current;
   pcb.running = pcb.ready;
   pcb.ready = pcb.running->next;
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
