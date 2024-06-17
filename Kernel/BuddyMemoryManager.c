#include "BuddyMemoryManager.h"
#include <stddef.h>
#include <stdint.h>
typedef struct BlockCDT {
    char *startAddress;
    size_t size;
    struct BlockCDT *nextBlock;
    char isFree;
} BlockCDT;

typedef struct MemoryManagerCDT {
    char *startAddress;
    uint64_t size;
    size_t spaceUsed;
    BlockCDT freeLists[POWER_OF_TWO_MAX_EXPONENT-MIN_EXP];
} MemoryManagerCDT;

MemoryManagerADT createMemoryManagerImpl(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);
void initManagerImpl(MemoryManagerADT manager);
void *allocMemoryImpl(MemoryManagerADT manager, size_t size);
void freeMemoryImpl(MemoryManagerADT manager, void *ptr);
void memStateImpl(MemoryManagerADT const restrict memoryManager, unsigned long long int *freeMemory, unsigned long long int *totalMemory, unsigned long long int *allocatedMemory);
int fast_log2(int X);
uint64_t pow2(int i);
void split_block(MemoryManagerADT  manager, int level);
void remove_from_free_list(MemoryManagerADT allocator, BlockADT block, int level);
void add_to_free_list(MemoryManagerADT allocator, BlockADT block, int level) ;


uint64_t pow2(int i) {
    // Handle negative values of i gracefully
    if (i < 0) {
        return 0;  // or handle error appropriately
    }

    // Shift 1 left by i positions to get 2^i
    return (uint64_t)1 << i;
}


MemoryManagerADT createMemoryManagerImpl(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
    MemoryManagerADT memoryManager = (MemoryManagerADT)memoryForMemoryManager;
    memoryManager->startAddress = (char *)managedMemory;
    memoryManager->spaceUsed = 0;
    memoryManager->size = USER_MEMORY_SIZE;
    initManagerImpl(memoryManager);
    return memoryManager;
}

void initManagerImpl(MemoryManagerADT manager) {
    for(int i = 0 ; i < POWER_OF_TWO_MAX_EXPONENT-MIN_EXP ; i++){
        manager->freeLists[i].isFree = FALSE;
        manager->freeLists[i].nextBlock = NULL;
        manager->freeLists[i].size = pow2(i+MIN_EXP);
        if(i != 0){
            manager->freeLists[i].startAddress = (char *)(manager->freeLists[i-1].startAddress + manager->freeLists[i-1].size + sizeof (BlockCDT));
        } else{
            manager->freeLists[i].startAddress = (char *)(manager->startAddress + sizeof (MemoryManagerCDT));
        }

    }
    manager->freeLists[POWER_OF_TWO_MAX_EXPONENT-1-MIN_EXP].isFree = TRUE ;
}

void split_block(MemoryManagerADT manager, int level) {
    BlockADT block = &manager->freeLists[level];
    remove_from_free_list(manager, block, level);

    size_t size = block->size / 2;

    // Calculate buddy block correctly
    BlockADT buddy = (BlockADT)((char *)block->startAddress + size + sizeof(BlockCDT));
    buddy->size = size;
    buddy->isFree = TRUE;
    buddy->nextBlock = NULL;

    block->size = size;

    add_to_free_list(manager, block, level - 1);
    add_to_free_list(manager, buddy, level - 1);
}


// Add a block to the free list
void add_to_free_list(MemoryManagerADT allocator, BlockADT block, int level) {
    block->nextBlock = &allocator->freeLists[level];
    allocator->freeLists[level] = *block;
}

// Remove a block from the free list
void remove_from_free_list(MemoryManagerADT allocator, BlockADT block, int level) {
    BlockADT current = &allocator->freeLists[level];

    while (current != NULL && current != block) {
        current = current->nextBlock;
    }

    if (current == block) {
        current = block->nextBlock;
    }
}



BlockADT recListSearch(MemoryManagerADT manager, int order, int split){
    BlockADT current = &manager->freeLists[order];
    BlockADT previous = NULL;
    while(current != NULL){
        if(current->isFree){
            if(previous!=NULL&&current->nextBlock!=NULL)
                previous->nextBlock=current->nextBlock;
            if(split){
                uint64_t aux = current->size;
                aux = aux/2;
                /*BlockADT block2=(BlockADT)current+aux;
                block2->startAddress=(char *)current+aux+sizeof(BlockCDT);
                block2->size=aux-sizeof(BlockCDT);
                block2->isFree=TRUE;
                block2->nextBlock=NULL;
                current->nextBlock=block2;*/
                split_block(manager, order);
                current->size=aux;

                current->isFree=FALSE;
                return current;
            } else {
                return current;
            }
        }else{
            if(current->nextBlock!=NULL)
                previous = current;
            current = current->nextBlock;
        }
    }
    BlockADT newBlock = recListSearch(manager,order+1,TRUE);
    if(previous!=NULL&&newBlock->nextBlock!=NULL)
        previous->nextBlock=newBlock->nextBlock;
    else if (previous==NULL)
        //Asignar el newBlock como el primero
        if(split){
            uint64_t aux = newBlock->size;
            aux = aux/2;
            split_block(manager, order);
            newBlock->size=aux;
            newBlock->isFree=FALSE;
        }
    return newBlock;
}



void *allocMemoryImpl(MemoryManagerADT manager, size_t size) {
    if (size > manager->size || size <= 0) {  //MEMO: ver si puedo alocarlo, yo me entiendo
        return NULL;
    }


    int bsize = fast_log2(size);

    return recListSearch(manager, bsize, 0);
    /*BlockADT current = &manager->freeLists[bsize];
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

    return block->startAddress;*/
}

void freeMemoryImpl(MemoryManagerADT manager, void *ptr) {
    /*if (ptr == NULL) {
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
    manager->freeLists[bsize] = block;*/
}

void memStateImpl(MemoryManagerADT const restrict memoryManager,
                  unsigned long long int *freeMemory,
                  unsigned long long int *totalMemory,
                  unsigned long long int *allocatedMemory) {
    if (memoryManager == NULL) {
        return;
    }
/*
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
	}*/
}
int fast_log2Buddy(int X) {
    if (X <= 0) {
        return -1;
    }
    int log2 = 0;
    // Use bit shifting to find the position of the highest set bit
    if (X >> 16) {
        X >>= 16;
        log2 += 16;
    }
    if (X >> 8) {
        X >>= 8;
        log2 += 8;
    }
    if (X >> 4) {
        X >>= 4;
        log2 += 4;
    }
    if (X >> 2) {
        X >>= 2;
        log2 += 2;
    }
    if (X >> 1) {
        log2 += 1;
    }
    log2 = log2-MIN_EXP;
    if(log2 < 0){
        log2 = 0;
    }
    return log2;
}

