#ifndef MEMMAN
#define MEMMAN
#include <stddef.h>

#define STARTING_ADRESS 0x50000
#define BLOCK_MAX_SIZE 4294967295

typedef struct MemoryManagerCDT *MemoryManagerADT;

void initialize_memory(void);
void *allocMemory(size_t size);
void freeMemory(void *ptr);
void memState(unsigned long long int * freeMemory, unsigned long long int * totalMemory, unsigned long long int * allocatedMemory);

#endif
