#include <memMan.h>

typedef struct MemoryManagerCDT {
    char *nextAddress;
    char *startAddress;
    size_t size;
    size_t spaceUsed;
} MemoryManagerCDT;

MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
    MemoryManagerADT memoryManager = (MemoryManagerADT) memoryForMemoryManager;
    memoryManager->nextAddress = managedMemory;
    memoryManager->spaceUsed = 0;
    memoryManager->size = 100000;
    memoryManager->startAddress = NULL; //  Donde termina el userspace.
    return memoryManager;
}

void *allocMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate) {

    if(memoryManager->spaceUsed + memoryToAllocate > memoryManager->size){
        return NULL;
    }
    else{
        char *allocation = memoryManager->nextAddress;
        memoryManager->nextAddress += memoryToAllocate;
        memoryManager->spaceUsed += memoryToAllocate;
        return (void *) allocation;
    }

}


void my_free(void * toFree){
    //memoryManager->nextAddress = memoryManager->startAddress;
    //memoryManager->spaceUsed = 0;
}
