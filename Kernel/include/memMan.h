#ifndef MEMMAN
#define MEMMAN
#include <stddef.h>
typedef struct MemoryManagerCDT * MemoryManagerADT;

void *allocMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate);
void my_free(void* toFree);
MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);


#endif