// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#if USE_BUDDY==1
#include <BuddyMemoryManager.h>
#include <stddef.h>

void initialize_memory(void) {
  init_mm((void *)0x0000000000600000, 0x0000000002700000);
}

void *allocMemory(size_t size) {
  return mallocBuddy(size); 
}

void freeMemory(void *ptr){
  freeBuddy(ptr);
}

void memState(unsigned long long int * freeMemory, unsigned long long int * totalMemory, unsigned long long int * allocatedMemory){
  buddyState((long unsigned int*)freeMemory,(long unsigned int*)totalMemory,(long unsigned int*)allocatedMemory);
}

#else
#include <BlockMemoryManager.h>

MemoryManagerADT systemMemory;

MemoryManagerADT
createMemoryManager(void *const restrict memoryForMemoryManager,
                    void *const restrict managedMemory) {
    return createMemoryManagerImpl(memoryForMemoryManager, managedMemory);
}

void initialize_memory(void) {
  systemMemory =
      createMemoryManager((void*)0x0000000000050000,(void*)0x0000000000500000);
}


void *allocMemory(size_t size) {
    if(size > BLOCK_MAX_SIZE)
        return NULL;
    return allocMemoryImpl(systemMemory, size); }

void freeMemory(void *ptr) { freeMemoryImpl(systemMemory, ptr); }

void memState(unsigned long long int *freeMemory,
              unsigned long long int *totalMemory,
              unsigned long long int *allocatedMemory) {
  memStateImpl(systemMemory, freeMemory, totalMemory, allocatedMemory);
}
#endif