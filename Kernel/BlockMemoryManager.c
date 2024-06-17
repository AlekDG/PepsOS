#include <BlockMemoryManager.h>
#include <lib.h>

#define USER_MEMORY_SIZE  0xFFFFFFFFFFFAFFFF

typedef struct MemoryManagerCDT {
  char *startAddress;
  unsigned long long int size;
  size_t spaceUsed;
  BlockADT firstBlock;
} MemoryManagerCDT;

typedef struct BlockCDT {
  char *startAddress;
  size_t size;
  size_t spaceUsed;
  struct BlockCDT *nextBlock;
  char isFree;
} BlockCDT;

void freeMemoryRec(MemoryManagerADT const restrict memoryManager,
                   void *memToFree, BlockADT currentBlock);
MemoryManagerADT
createMemoryManagerImpl(void *const restrict memoryForMemoryManager,
                        void *const restrict managedMemory);
BlockADT createBlock(void *startAddress, size_t size, BlockADT currentBlock,
                     BlockADT *result);
void memStateImpl(MemoryManagerADT const restrict memoryManager,
                  unsigned long long int *freeMemory,
                  unsigned long long int *totalMemory,
                  unsigned long long int *allocatedMemory);

void memStateRec(unsigned long long int *freeMemory,
                 unsigned long long int *allocatedMemory,
                 BlockADT currentBlock);
void coalesceFreeBlocks(MemoryManagerADT memoryManager);
void coalesceFreeBlocksRec(BlockADT currentBlock);

MemoryManagerADT
createMemoryManagerImpl(void *const restrict memoryForMemoryManager,
                        void *const restrict managedMemory) {
  MemoryManagerADT memoryManager = (MemoryManagerADT)memoryForMemoryManager;
  memoryManager->startAddress = managedMemory;
  memoryManager->spaceUsed = 0;
  memoryManager->size = USER_MEMORY_SIZE;
  memoryManager->firstBlock = NULL;
  return memoryManager;
}

BlockADT createBlock(void *startAddress, size_t size, BlockADT currentBlock,
                     BlockADT *result) {
  if (currentBlock != NULL) {
    if (currentBlock->isFree == 1) {
      if (currentBlock->size >= size) {
        currentBlock->spaceUsed = 0;
        currentBlock->isFree = 0;
        *result = currentBlock;
        return currentBlock;
      }
    }
    currentBlock->nextBlock =
        createBlock(startAddress, size, currentBlock->nextBlock, result);
    return currentBlock;
  }

  BlockADT newBlock = (BlockADT)startAddress;
  newBlock->startAddress = startAddress + sizeof(BlockCDT);
  newBlock->size = size;
  newBlock->spaceUsed = 0;
  newBlock->nextBlock = NULL;
  newBlock->isFree = 0;
  *result = newBlock;
  return newBlock;
}

void *allocMemoryImpl(MemoryManagerADT const restrict memoryManager,
                      const size_t memoryToAllocate) {
  if (memoryManager->spaceUsed + memoryToAllocate <= memoryManager->size) {
    BlockADT block = NULL;
    memoryManager->firstBlock =
        createBlock(memoryManager->startAddress + memoryManager->spaceUsed + 1,
                    memoryToAllocate, memoryManager->firstBlock, &block);
    block->isFree = 0;
    memoryManager->spaceUsed += memoryToAllocate + sizeof(BlockCDT);
    return block->startAddress;
  }
  return NULL;
}

void freeMemoryImpl(MemoryManagerADT const restrict memoryManager,
                    void *memToFree) {
  if (memoryManager != NULL && memToFree != NULL) {
    freeMemoryRec(memoryManager, memToFree, memoryManager->firstBlock);
  }
  coalesceFreeBlocks(memoryManager);
}

void freeMemoryRec(MemoryManagerADT const restrict memoryManager,
                   void *memToFree, BlockADT currentBlock) {
  if (memToFree == currentBlock->startAddress ||
      memToFree < (void *)(currentBlock->startAddress + currentBlock->size)) {
    currentBlock->isFree = 1;
  } else {
    freeMemoryRec(memoryManager, memToFree, currentBlock->nextBlock);
  }
}

void memStateRec(unsigned long long int *freeMemory,
                 unsigned long long int *allocatedMemory,
                 BlockADT currentBlock) {
  if (currentBlock == NULL) {
    return;
  }
  if (currentBlock->isFree) {
    *freeMemory += currentBlock->size;
  } else {
    *allocatedMemory += currentBlock->size;
  }
  memStateRec(freeMemory, allocatedMemory, currentBlock->nextBlock);
}

void memStateImpl(MemoryManagerADT const restrict memoryManager,
                  unsigned long long int *freeMemory,
                  unsigned long long int *totalMemory,
                  unsigned long long int *allocatedMemory) {
  if (memoryManager == NULL) {
    return;
  }
  *totalMemory = memoryManager->size;
  memStateRec(freeMemory, allocatedMemory, memoryManager->firstBlock);
}

void coalesceFreeBlocksRec(BlockADT currentBlock) {
  if (currentBlock == NULL || currentBlock->nextBlock == NULL) {
    return;
  }

  if (currentBlock->isFree && currentBlock->nextBlock->isFree) {
    currentBlock->size += currentBlock->nextBlock->size + sizeof(BlockCDT);
    currentBlock->nextBlock = currentBlock->nextBlock->nextBlock;
    coalesceFreeBlocksRec(currentBlock);
  } else {
    coalesceFreeBlocksRec(currentBlock->nextBlock);
  }
}

void coalesceFreeBlocks(MemoryManagerADT const restrict memoryManager) {
  if (memoryManager == NULL) {
    return;
  }
  coalesceFreeBlocksRec(memoryManager->firstBlock);
}
