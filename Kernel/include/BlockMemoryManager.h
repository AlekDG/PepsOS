// BlockMemoryManager.h
#ifndef BLOCK_MEMORY_MANAGER_H
#define BLOCK_MEMORY_MANAGER_H

#include "memMan.h"

MemoryManagerADT
createMemoryManagerImpl(void *const restrict memoryForMemoryManager,
                        void *const restrict managedMemory);
void initManagerImpl(MemoryManagerADT manager);
void *allocMemoryImpl(MemoryManagerADT manager, size_t size);
void freeMemoryImpl(MemoryManagerADT manager, void *ptr);
void memStateImpl(MemoryManagerADT const restrict memoryManager, unsigned long long int * freeMemory,
                  unsigned long long int * totalMemory, unsigned long long int * allocatedMemory);

void memStateRec(unsigned long long int * freeMemory, unsigned long long int * allocatedMemory, BlockADT currentBlock);

#endif // BLOCK_MEMORY_MANAGER_H
