#ifndef MEMMAN
#define MEMMAN
#include <stddef.h>
typedef struct MemoryManagerCDT * MemoryManagerADT;
typedef struct BlockCDT * BlockADT;

void *allocMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate);
void my_free(void* toFree);
MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);

void initManager(MemoryManagerADT manager);
void *allocMemory(MemoryManagerADT manager, size_t size);
void freeMemory(MemoryManagerADT manager, void *ptr);

#endif


