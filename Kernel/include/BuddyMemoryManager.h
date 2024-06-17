
#ifndef BUDDY_MEMORY_MANAGER_H
#define BUDDY_MEMORY_MANAGER_H

#include "memMan.h"

#define TRUE 1
#define FALSE 0
#define POWER_OF_TWO_MAX_EXPONENT 25
#define MIN_EXP 12
typedef struct MemoryManagerCDT *MemoryManagerADT;
MemoryManagerADT createMemoryManagerImpl(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);
void initManagerImpl(MemoryManagerADT manager);
void *allocMemoryImpl(MemoryManagerADT manager, size_t size);
void freeMemoryImpl(MemoryManagerADT manager, void *ptr);
void memStateImpl(MemoryManagerADT const restrict memoryManager, unsigned long long int * freeMemory, unsigned long long int * totalMemory, unsigned long long int * allocatedMemory);

#endif
