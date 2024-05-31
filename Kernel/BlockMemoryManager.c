//
// Created by jo on 17/05/24.
//
#include "BlockMemoryManager.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define BUDDY 0
#define BUDDY_MEM_BLOCK_SIZE 32

typedef struct MemoryManagerCDT {
    char *startAddress;
    size_t size;
    size_t spaceUsed;
    BlockADT firstBlock;
#ifdef BUDDY
    BlockADT otherFirstBlock;
    size_t maxMem;
#endif
} MemoryManagerCDT;

typedef struct BlockCDT{
    char * startAddress;
    size_t size;
    size_t spaceUsed;
    struct BlockCDT * nextBlock;
#ifdef BUDDY
    struct BlockCDT * otherNextBlock; //	"The reasons for wanting to do this though, are questionable."
#endif
    char isFree;
}BlockCDT;

//	Deberia haber un MemoryManagerADT global que se llame en todas las funciones. StartAddress tendria que ser donde termina el user space.

void freeMemoryRec(MemoryManagerADT const restrict memoryManager, void * memToFree, BlockADT currentBlock);
BlockADT createBuddyBlock(int size, BlockADT currentBlock, void * startAddress);
void initiateBuddySystem(MemoryManagerADT const restrict memoryManager);
MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);
BlockADT createBlock(void * startAddress, size_t size, BlockADT currentBlock,BlockADT * result);


MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
    MemoryManagerADT memoryManager = (MemoryManagerADT) memoryForMemoryManager;
    memoryManager->startAddress = managedMemory; //  Donde termina el userspace.
    memoryManager->spaceUsed = 0;
    memoryManager->size = 100000;
    memoryManager->firstBlock = NULL;
    return memoryManager;
}

#ifdef BUDDY
void initiateBuddySystem(MemoryManagerADT const restrict memoryManager){
    memoryManager->maxMem = 250;
    memoryManager->firstBlock = createBuddyBlock(memoryManager->maxMem, memoryManager->firstBlock, memoryManager->startAddress);
    memoryManager->otherFirstBlock = createBuddyBlock(memoryManager->maxMem, memoryManager->otherFirstBlock, memoryManager->startAddress);
}
#endif

#ifdef BUDDY
BlockADT createBuddyBlock(int size, BlockADT currentBlock, void * startAddress){
    if(size <= BUDDY_MEM_BLOCK_SIZE){
        return NULL;
    }
    if(currentBlock == NULL){
        BlockADT newBlock = (BlockADT) startAddress;
        newBlock->startAddress = startAddress + sizeof(BlockCDT);
        //+ sizeof newBlock->startAddress +
        //	 sizeof newBlock->size + sizeof newBlock->spaceUsed + sizeof newBlock->nextBlock + sizeof newBlock->isFree + sizeof newBlock->otherNextBlock;
        newBlock->size = size;
        newBlock->spaceUsed = 0;
        newBlock->isFree = 1;
        newBlock->nextBlock = createBuddyBlock(size / 2, NULL, (char*)newBlock->startAddress + size);
        newBlock->otherNextBlock = createBuddyBlock(size / 2, NULL, (char*)newBlock->startAddress + size);
        return newBlock;
    }
    else{
        return NULL;
    }
}
#endif

BlockADT createBlock(void * startAddress, size_t size, BlockADT currentBlock, BlockADT* result ){
    if(currentBlock != NULL){
        if(currentBlock->isFree == 1){
            //	Si soy un bloque libre de size suficiente, escribir.
            if(currentBlock->size >= size){
                currentBlock->startAddress = startAddress + sizeof currentBlock->startAddress +
                                             sizeof currentBlock->size + sizeof currentBlock->spaceUsed + sizeof currentBlock->nextBlock + sizeof currentBlock->isFree;
                currentBlock->size = size;
                currentBlock->spaceUsed = 0;
                //	El siguiente bloque no cambia!!
                currentBlock->isFree = 0;

                return currentBlock;
            }
        }
        else{
            currentBlock->nextBlock = createBlock(startAddress, size, currentBlock->nextBlock,result);
            return currentBlock;
        }
        return currentBlock;
    }

    else{
        BlockADT newBlock = (BlockADT) startAddress;
        newBlock->startAddress = startAddress + sizeof newBlock->startAddress +
                                 sizeof newBlock->size + sizeof newBlock->spaceUsed + sizeof newBlock->nextBlock + sizeof newBlock->isFree;
        //	esto es feo como pecado de fraile pero sirve.
        newBlock->size = size;
        newBlock->spaceUsed = 0;
        newBlock->nextBlock = NULL;
        newBlock->isFree = 0;
        *result = newBlock;
        return newBlock;
    }
}

void *allocMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate) {
    if(memoryManager->spaceUsed + memoryToAllocate > memoryManager->size){
        return NULL;
    }
    else{
        //  Retorno el puntero al inicio de un nuevo bloque de memoria.
        BlockADT block = NULL;
        createBlock(memoryManager->startAddress + memoryManager->spaceUsed + 1, memoryToAllocate, memoryManager->firstBlock,&block);
        block->isFree = 0;
        memoryManager->firstBlock = block;
        memoryManager->spaceUsed += memoryToAllocate + sizeof block->startAddress +
                                 sizeof block->size + sizeof block->spaceUsed + sizeof block->nextBlock + sizeof block->isFree;;
        return block->startAddress;
    }
}

void freeMemory(MemoryManagerADT const restrict memoryManager, void * memToFree){
    if(memoryManager == NULL || memToFree == NULL){
        return;
    }
    else{
        freeMemoryRec(memoryManager, memToFree, memoryManager->firstBlock);
    }
}

void freeMemoryRec(MemoryManagerADT const restrict memoryManager, void * memToFree, BlockADT currentBlock) {
    if (memToFree == currentBlock->startAddress ||
        memToFree < (void *) (currentBlock->startAddress + currentBlock->size)) {
        currentBlock->isFree = 1;
    }
    else {
        freeMemoryRec(memoryManager, memToFree, currentBlock->nextBlock);
    }
}

