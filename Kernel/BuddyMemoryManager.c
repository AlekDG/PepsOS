#include <BuddyMemoryManager.h>
#include <memMan.h>
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
    BlockCDT freeLists[POWER_OF_TWO_MAX_EXPONENT];
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
    initManagerImpl(memoryManager);
    return memoryManager;
}

void initManagerImpl(MemoryManagerADT manager) {
    for(int i = 0 ; i < POWER_OF_TWO_MAX_EXPONENT -2 ; i++){
        manager->freeLists[i].isFree = FALSE;
    }
    manager->freeLists[POWER_OF_TWO_MAX_EXPONENT-1].isFree = TRUE ;
}

void *allocMemoryImpl(MemoryManagerADT manager, size_t size) {
    if (size > manager->size || size <= 0) {  //MEMO: ver si puedo alocarlo, yo me entiendo
        return NULL;
    }


    int bsize = fast_log2(size)+1;

    BlockADT current = &manager->freeLists[bsize];
    while(current != NULL){
        if(current->isFree){
            //hacemos devolvemos este
        }else{
            current = current->nextBlock;
        }
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
