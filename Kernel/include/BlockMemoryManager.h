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
void memStateImpl(MemoryManagerADT const restrict memoryManager, size_t * freeMemory,
                  size_t * totalMemory, size_t * allocatedMemory);

void memStateRec(size_t * freeMemory, size_t * allocatedMemory, BlockADT currentBlock);

#endif // BLOCK_MEMORY_MANAGER_H
