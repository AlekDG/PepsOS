// BlockMemoryManager.h
#ifndef BLOCK_MEMORY_MANAGER_H
#define BLOCK_MEMORY_MANAGER_H

#include "memMan.h"

MemoryManagerADT createMemoryManagerImpl(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);
void initManagerImpl(MemoryManagerADT manager);
void *allocMemoryImpl(MemoryManagerADT manager, size_t size);
void freeMemoryImpl(MemoryManagerADT manager, void *ptr);

#endif // BLOCK_MEMORY_MANAGER_H
