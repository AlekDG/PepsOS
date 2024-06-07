// BuddyMemoryManager.h
#ifndef BUDDY_MEMORY_MANAGER_H
#define BUDDY_MEMORY_MANAGER_H

#include "memMan.h"

#define TRUE 1
#define FALSE 0

typedef struct MemoryManagerCDT *MemoryManagerADT;
typedef MemoryManagerADT
createMemoryManagerImpl(void *const restrict memoryForMemoryManager,
                        void *const restrict managedMemory);
void initManagerImpl(MemoryManagerADT manager);
void *allocMemoryImpl(MemoryManagerADT manager, size_t size);
void freeMemoryImpl(MemoryManagerADT manager, void *ptr);

#endif // BUDDY_MEMORY_MANAGER_H
