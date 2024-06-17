#ifndef MEMMAN
#define MEMMAN
#include <stddef.h>

#define STARTING_ADRESS 0x50000
#ifdef USE_BUDDY
#define USER_MEMORY_SIZE  0x24FFFFF
#else
#define BLOCK_MAX_SIZE 4294967295 //    biggest number that fits in a size_t

typedef struct MemoryManagerCDT *MemoryManagerADT;
typedef struct BlockCDT *BlockADT;
#define USER_MEMORY_SIZE  0xFFFFFFFFFFFAFFFF
#endif

void initialize_memory(void);
void *allocMemory(size_t size);
void freeMemory(void *ptr);
void memState(unsigned long long int * freeMemory, unsigned long long int * totalMemory, unsigned long long int * allocatedMemory);

#endif
