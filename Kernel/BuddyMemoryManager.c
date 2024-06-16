#include <BuddyMemoryManager.h>
#include <lib.h>


typedef struct MemoryManagerCDT {
  char *startAddress;
  size_t size;
  size_t spaceUsed;
  BlockADT firstBlock;
  BlockADT freeLists[POWER_OF_TWO_MAX_EXPONENT];
} MemoryManagerCDT;

typedef struct BlockCDT {
  char *startAddress;
  size_t size;
  struct BlockCDT *nextBlock;
  char isFree;
} BlockCDT;

MemoryManagerADT
createMemoryManagerImpl(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);
void initManagerImpl(MemoryManagerADT manager);
void *allocMemoryImpl(MemoryManagerADT manager, size_t size);

MemoryManagerADT createMemoryManagerImpl(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
  MemoryManagerADT memoryManager = (MemoryManagerADT)memoryForMemoryManager;
  memoryManager->startAddress = managedMemory;
  memoryManager->spaceUsed = 0;
  memoryManager->size = USER_MEMORY_SIZE;
  memoryManager->firstBlock = NULL;
  return memoryManager;
}

void initManagerImpl(MemoryManagerADT manager) {
  manager->firstBlock = (BlockADT)manager->startAddress;
  manager->firstBlock->size = manager->size;
  manager->firstBlock->isFree = TRUE;
  manager->firstBlock->nextBlock = NULL;

  for (int i = 0; i < POWER_OF_TWO_MAX_EXPONENT; i++) {
    manager->freeLists[i] = NULL;
  }
}

int findPower2(int size) {
  int block_size = 1;
  while (block_size < size) {
    block_size *= 2;
  }
  return block_size;
}

void *allocMemoryImpl(MemoryManagerADT manager, size_t size) {
  if (size > manager->size || size <= 0) {
    return NULL;
  }

  int blockSize = findPower2(size);
  int bsize;

  bsize = fast_log2(blockSize);
  if (manager->freeLists[bsize] != NULL) {
    BlockADT block = manager->freeLists[bsize];
    block->isFree = FALSE;
    manager->freeLists[bsize] = block->nextBlock;
    return block->startAddress;
  }
      BlockADT newBlock = manager->freeLists[bsize];
      newBlock->startAddress = manager->freeLists[bsize]->startAddress + sizeof(BlockCDT);
      newBlock->size = blockSize;
      newBlock->isFree = FALSE;
      newBlock->nextBlock = NULL;
      manager->spaceUsed += blockSize;



  while (newBlock->size > size * 2) {
    newBlock->size /= 2;
      BlockADT buddyBlock =  manager->freeLists[bsize];
      buddyBlock->startAddress = manager->freeLists[bsize]->startAddress + sizeof(BlockCDT);
    buddyBlock->size = newBlock->size;
    buddyBlock->isFree = TRUE;
    buddyBlock->nextBlock = manager->freeLists[fast_log2(buddyBlock->size)];
    manager->freeLists[fast_log2(buddyBlock->size)]->nextBlock = buddyBlock;
  }

  if (manager->freeLists[bsize] == NULL) {
    manager->freeLists[bsize] = newBlock;
  } else {
    BlockADT currentBlock = manager->freeLists[bsize];
    while (currentBlock->nextBlock != NULL) {
      currentBlock = currentBlock->nextBlock;
    }
    currentBlock->nextBlock = newBlock;
  }
  return newBlock->startAddress;
}

BlockADT findBlock(MemoryManagerADT manager, void *ptr) {
  for (int i = 0; i < POWER_OF_TWO_MAX_EXPONENT; i++) {
    BlockADT currentBlock = manager->freeLists[i];
    while (currentBlock != NULL) {
      if (currentBlock->startAddress == ptr) {
        return currentBlock;
      }
      currentBlock = currentBlock->nextBlock;
    }
  }

  return NULL;
}

void freeMemoryImpl(MemoryManagerADT manager, void *ptr) {
  BlockADT block = findBlock(manager, ptr);
  if (block == NULL || block->isFree) {
    return;
  }

  block->isFree = TRUE;
  int bsize = fast_log2(block->size);
  BlockADT buddyBlock;

  while (block->size < manager->size) {
    size_t offset = block->startAddress - manager->startAddress;
    size_t buddyAddress =
        (offset ^ block->size) + (size_t)manager->startAddress;
    buddyBlock = findBlock(manager, (void *)buddyAddress);

    if (buddyBlock && buddyBlock->isFree && buddyBlock->size == block->size) {
      BlockADT *freeList = &manager->freeLists[bsize];
      while (*freeList != NULL && *freeList != buddyBlock) {
        freeList = &(*freeList)->nextBlock;
      }
      if (*freeList == buddyBlock) {
        *freeList = buddyBlock->nextBlock;
      }

      if (block->startAddress < buddyBlock->startAddress) {
        block->size *= 2;
      } else {
        block = buddyBlock;
        block->size *= 2;
      }
      bsize++;
    } else {
      break;
    }
  }

  if (buddyBlock != NULL)
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
  *freeMemory = 0;
  *totalMemory = memoryManager->size;

  for (int i = 0; i < POWER_OF_TWO_MAX_EXPONENT; i++) {
    BlockADT currentBlock = memoryManager->freeLists[i];
    while (currentBlock != NULL) {
      *freeMemory += currentBlock->size;
      currentBlock = currentBlock->nextBlock;
    }
  }
  *allocatedMemory = *totalMemory - *freeMemory;
}