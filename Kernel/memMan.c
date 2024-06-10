// Define which memory manager to use
#ifdef USE_BUDDY
#include <BuddyMemoryManager.h>
#else
#include <BlockMemoryManager.h>
#endif

MemoryManagerADT systemMemory;

void initialize_memory(void) {
  systemMemory =
      createMemoryManager(0x0000000000050000, (void *)0x0000000000500000);
}

MemoryManagerADT
createMemoryManager(void *const restrict memoryForMemoryManager,
                    void *const restrict managedMemory) {
  return createMemoryManagerImpl(memoryForMemoryManager, managedMemory);
}

//void initManager(void) { initManagerImpl(systemMemory); }

void *allocMemory(size_t size) { return allocMemoryImpl(systemMemory, size); }

void freeMemory(void *ptr) { freeMemoryImpl(systemMemory, ptr); }

void memState(size_t * freeMemory, size_t * totalMemory, size_t * allocatedMemory){
    memStateImpl(systemMemory, freeMemory, totalMemory, allocatedMemory);
}
