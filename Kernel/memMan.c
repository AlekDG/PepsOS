// MemoryManager.c

// Define which memory manager to use
#ifdef USE_BUDDY
#include <BuddyMemoryManager.h>
#else
#include <BlockMemoryManager.h>
#endif

MemoryManagerADT
createMemoryManager(void *const restrict memoryForMemoryManager,
                    void *const restrict managedMemory) {
  return createMemoryManagerImpl(memoryForMemoryManager, managedMemory);
}

void initManager(MemoryManagerADT manager) { initManagerImpl(manager); }

void *allocMemory(MemoryManagerADT manager, size_t size) {
  return allocMemoryImpl(manager, size);
}

void freeMemory(MemoryManagerADT manager, void *ptr) {
  freeMemoryImpl(manager, ptr);
}
