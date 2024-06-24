#include <stddef.h>

#define MEMORY_SIZE 1024
//0 if freem 1 if allocd
int allocMap[MEMORY_SIZE];
int allocCount=0;
void *memory[MEMORY_SIZE];

void *defAlloc(void){
    if(allocCount=MEMORY_SIZE)
        return NULL;
    allocMap[allocCount]=1;
    return memory[allocCount++];
}

void defFree(void* ptr){
    int i=0;
    for(;i<MEMORY_SIZE&&memory[i]<ptr;i++);
    allocMap[i]=0;
    return;
}