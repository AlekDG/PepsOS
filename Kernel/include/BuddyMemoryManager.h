#ifndef BUDDYMEMORYMANAGER_H
#define BUDDYMEMORYMANAGER_H

#define TRUE 1
#define FALSE 0

typedef struct BlockCDT * BlockADT;
typedef struct MemoryManagerCDT * MemoryManagerADT;
MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);

#endif // BUDDYMEMORYMANAGER_H
