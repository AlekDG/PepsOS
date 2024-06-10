#ifndef MEMMAN
#define MEMMAN
#include <stddef.h>

#define STARTING_ADRESS 0x50000

typedef struct MemoryManagerCDT *MemoryManagerADT;
typedef struct BlockCDT *BlockADT;

void initialize_memory(void);

MemoryManagerADT
createMemoryManager(void *const restrict memoryForMemoryManager,
                    void *const restrict managedMemory);

//void initManager(void);
void *allocMemory(size_t size);
void freeMemory(void *ptr);
void memState(size_t * freeMemory, size_t * totalMemory, size_t * allocatedMemory);

#endif
