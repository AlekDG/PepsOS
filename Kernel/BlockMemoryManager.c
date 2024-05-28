//
// Created by jo on 17/05/24.
//
#include "BlockMemoryManager.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct MemoryManagerCDT {
    char *startAddress;
    size_t size;
    size_t spaceUsed;
    BlockADT firstBlock;
} MemoryManagerCDT;

typedef struct BlockCDT{
    char * startAddress;
    size_t size;
    size_t spaceUsed;
    struct BlockCDT * nextBlock;
    char isFree;
}BlockCDT;

//	Deberia haber un MemoryManagerADT global que se llame en todas las funciones. StartAddress tendria que ser donde termina el user space.

void freeMemoryRec(MemoryManagerADT const restrict memoryManager, void * memToFree, BlockADT currentBlock);

MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
    MemoryManagerADT memoryManager = (MemoryManagerADT) memoryForMemoryManager;
    memoryManager->startAddress = managedMemory; //  Donde termina el userspace.
    memoryManager->spaceUsed = 0;
    memoryManager->size = 100000;
    memoryManager->firstBlock = NULL;
    return memoryManager;
}

BlockADT createBlock(void * startAddress, size_t size, BlockADT currentBlock){
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
			currentBlock->nextBlock = createBlock(startAddress, size, currentBlock->nextBlock);
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
		return newBlock;
    }
}

void *allocMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate) {
    if(memoryManager->spaceUsed + memoryToAllocate > memoryManager->size){
        return NULL;
    }
    else{
        //  Retorno el puntero al inicio de un nuevo bloque de memoria.
        BlockADT block = createBlock(memoryManager->startAddress + memoryManager->spaceUsed + 1, memoryToAllocate, memoryManager->firstBlock);
        block->isFree = 0;
		memoryManager->firstBlock = block;
		memoryManager->spaceUsed += memoryToAllocate;
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

void freeMemoryRec(MemoryManagerADT const restrict memoryManager, void * memToFree, BlockADT currentBlock){
	if(memToFree == currentBlock->startAddress || memToFree < (void *)(currentBlock->startAddress + currentBlock->size)){
		currentBlock->isFree = 1;
	}
	else{
		freeMemoryRec(memoryManager, memToFree, currentBlock->nextBlock);
	}
}


int main(){
	void * memoryForManager = malloc(1000);
	void * managedMem = malloc(1000);
	MemoryManagerADT memoryManager = createMemoryManager(memoryForManager, managedMem);
	void * newArr = allocMemory(memoryManager, 100);
	void * otherArr = allocMemory(memoryManager, 12);
	int * int_arr = (int *) newArr;
	for(int i = 0; i < 10; i++){
		int_arr[i] = i;
	}
	char * char_arr = (char *) otherArr;
	for(char i = 0; i < 10; i++){
		char_arr[i] = 'a'+i;
	}
	freeMemory(memoryManager, newArr);
	void * aThirdArr = allocMemory(memoryManager, 50);
	free(memoryForManager);
	free(managedMem);
	free(newArr);
	free(int_arr);
	return 0;
}