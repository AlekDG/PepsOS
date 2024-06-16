#include "BuddyMemoryManager.h"
#include "memMan.h"
#include <lib.h>
typedef struct BlockCDT {
    char *startAddress;
    size_t size;
    struct BlockCDT *nextBlock;
    char isFree;
} BlockCDT;

typedef struct MemoryManagerCDT {
    char *startAddress;
    size_t size;
    size_t spaceUsed;
    BlockADT firstBlock;
    BlockADT freeLists[POWER_OF_TWO_MAX_EXPONENT];
} MemoryManagerCDT;

MemoryManagerADT createMemoryManagerImpl(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);
void initManagerImpl(MemoryManagerADT manager);
void *allocMemoryImpl(MemoryManagerADT manager, size_t size);
void freeMemoryImpl(MemoryManagerADT manager, void *ptr);
void memStateImpl(MemoryManagerADT const restrict memoryManager, unsigned long long int *freeMemory, unsigned long long int *totalMemory, unsigned long long int *allocatedMemory);


MemoryManagerADT createMemoryManagerImpl(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
    MemoryManagerADT memoryManager = (MemoryManagerADT)memoryForMemoryManager;
    memoryManager->startAddress = (char *)managedMemory;
    memoryManager->spaceUsed = 0;
    memoryManager->size = USER_MEMORY_SIZE;
    memoryManager->firstBlock = NULL;
    initManagerImpl(memoryManager);
    return memoryManager;
}

void initManagerImpl(MemoryManagerADT manager) {
    manager->firstBlock = (BlockADT)manager->startAddress;
    manager->firstBlock->size = manager->size;
    manager->firstBlock->isFree = TRUE;
    manager->firstBlock->nextBlock = NULL;
    int maxBlocks = log2_fast_long(USER_MEMORY_SIZE);
    int blockSize = 1;
    for (int i = 0; i < POWER_OF_TWO_MAX_EXPONENT; i++) {
        for (int j = 0; j < maxBlocks; j++) {
            BlockADT newBlock = (BlockADT)(manager->startAddress + manager->spaceUsed);
            newBlock->startAddress = manager->startAddress + manager->spaceUsed + sizeof(BlockCDT);
            newBlock->size = blockSize;
            newBlock->isFree = TRUE;
            manager->spaceUsed += sizeof(BlockCDT);
            newBlock->nextBlock = manager->freeLists[i];
            manager->freeLists[i] = newBlock;
        }
        blockSize *= 2;
        maxBlocks--;
        if (maxBlocks < 0) {
            maxBlocks = 0;
        }
    }
}

void *allocMemoryImpl(MemoryManagerADT manager, size_t size) {
    if (size > manager->size || size <= 0) {
        return NULL;
    }

    int blockSize = (1 << fast_log2(size));
    int bsize = fast_log2(blockSize);

    while (bsize < POWER_OF_TWO_MAX_EXPONENT && manager->freeLists[bsize] == NULL) {
        bsize++;
        blockSize *= 2;
    }

    if (bsize >= POWER_OF_TWO_MAX_EXPONENT) {
        return NULL;
    }

    BlockADT block = manager->freeLists[bsize];
    manager->freeLists[bsize] = block->nextBlock;
    block->isFree = FALSE;

    while (blockSize > size) {
        blockSize /= 2;
        bsize--;
        BlockADT buddyBlock = (BlockADT)((block->startAddress - manager->startAddress) + blockSize + manager->startAddress);
        buddyBlock->size = blockSize;
        buddyBlock->isFree = TRUE;
        buddyBlock->nextBlock = manager->freeLists[bsize];
        manager->freeLists[bsize] = buddyBlock;
    }

    return block->startAddress;
}

void freeMemoryImpl(MemoryManagerADT manager, void *ptr) {
    if (ptr == NULL) {
        return;
    }

    BlockADT block = (BlockADT)((char *)ptr - sizeof(BlockCDT));
    block->isFree = TRUE;
    int bsize = fast_log2(block->size);

    while (bsize < POWER_OF_TWO_MAX_EXPONENT - 1) {
        size_t offset = (block->startAddress - manager->startAddress);
        size_t buddyAddress = offset ^ block->size;
        BlockADT buddyBlock = (BlockADT)(buddyAddress + manager->startAddress);

        if (buddyBlock->isFree && buddyBlock->size == block->size) {
            if (buddyBlock->startAddress < block->startAddress) {
                BlockADT tempBlock = block;
                block = buddyBlock;
                buddyBlock = tempBlock;
            }
            manager->freeLists[bsize] = buddyBlock->nextBlock;
            block->size *= 2;
            bsize++;
        } else {
            break;
        }
    }

    block->nextBlock = manager->freeLists[bsize];
    manager->freeLists[bsize] = block;
}

void memStateImpl(MemoryManagerADT const restrict memoryManager,
                  unsigned long long int *freeMemory,
                  unsigned long long int *totalMemory,
                  unsigned long long int *allocatedMemory) {
    if (memoryManager == NULL) {
        return;
    }

    *totalMemory = memoryManager->size;
    *freeMemory = 0;
    *allocatedMemory = 0;

    for (int i = 0; i < POWER_OF_TWO_MAX_EXPONENT; i++) {
        BlockADT currentBlock = memoryManager->freeLists[i];
        while (currentBlock != NULL) {
            if (currentBlock->isFree) {
                *freeMemory += currentBlock->size;
            } else {
                *allocatedMemory += currentBlock->size;
            }
            currentBlock = currentBlock->nextBlock;
        }
    }
}

