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
void memState(unsigned long long int * freeMemory, unsigned long long int * totalMemory, unsigned long long int * allocatedMemory);

#endif
