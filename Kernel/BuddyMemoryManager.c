//
// Created by jo on 03/06/24.
//

#include "include/BuddyMemoryManager.h"
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stddef.h>

#define POWER_OF_TWO_MAX_EXPONENT 32

typedef struct MemoryManagerCDT {
	char *startAddress;
	size_t size;
	size_t spaceUsed;
	BlockADT firstBlock;
	BlockADT freeLists[POWER_OF_TWO_MAX_EXPONENT];		//	Array to store free lists of different sizes. Each index corresponds to a list of blocks of size 2^index.
} MemoryManagerCDT;

typedef struct BlockCDT{
	char * startAddress;
	size_t size;
	struct BlockCDT * nextBlock;
	char isFree;
}BlockCDT;

MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);
void initManager(MemoryManagerADT manager);
void * allocMemory(MemoryManagerADT manager, int size);

	MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
	MemoryManagerADT memoryManager = (MemoryManagerADT) memoryForMemoryManager;
	memoryManager->startAddress = managedMemory; //  Donde termina el userspace.
	memoryManager->spaceUsed = 0;
	memoryManager->size = 100000;
	memoryManager->firstBlock = NULL;
	return memoryManager;
}

void initManager(MemoryManagerADT manager) {
	manager->firstBlock = (BlockADT) manager->startAddress;
	manager->firstBlock->size = manager->size;
	manager->firstBlock->isFree = TRUE;
	manager->firstBlock->nextBlock = NULL;

	for (int i = 0; i < POWER_OF_TWO_MAX_EXPONENT; i++) {
		manager->freeLists[i] = NULL;
	}
}

int findPower2(int size){
	int block_size = 1;
	while (block_size < size) {
		block_size *= 2;
	}
	return block_size;
}

void * allocMemory(MemoryManagerADT manager, int size) {
	if (size > manager->size || size <= 0){
		return NULL;
	}

	int blockSize = findPower2(size);
	int bsize;
	// Search for a free block of the required size or larger

	bsize = log2(blockSize);
	if (manager->freeLists[bsize] != NULL) {
		// Found a free block of the required size or larger
		BlockADT block = manager->freeLists[bsize];
		block->isFree = FALSE;
		manager->freeLists[bsize] = block->nextBlock;
		return block->startAddress;
	}


	// Unable to find a suitable free block, create a new block

	//	SIZE MATTERS
	BlockADT newBlock = (BlockADT) ((char*)manager->startAddress + manager->spaceUsed);
	newBlock->startAddress = (void *)newBlock + manager->spaceUsed+sizeof newBlock + sizeof newBlock->startAddress + sizeof newBlock->size + sizeof newBlock->isFree + sizeof newBlock->nextBlock;

	newBlock->size = blockSize;
	newBlock->isFree = FALSE;
	newBlock->nextBlock = NULL;
	manager->spaceUsed += blockSize;

	// Split the new block into smaller blocks if necessary
	while (newBlock->size > size * 2) { // Split only if the block size is more than twice the requested size
		newBlock->size /= 2;
		BlockADT buddyBlock = (BlockADT) ((void*)newBlock + sizeof(BlockCDT) + newBlock->size); // Calculate buddy block address
		buddyBlock->startAddress = (void *) buddyBlock + manager->spaceUsed+sizeof newBlock + sizeof newBlock->startAddress + sizeof newBlock->size + sizeof newBlock->isFree + sizeof newBlock->nextBlock;
		buddyBlock->size = newBlock->size;
		buddyBlock->isFree = TRUE;
		buddyBlock->nextBlock = manager->freeLists[(int)log2(buddyBlock->size)];
		manager->freeLists[(int)log2(buddyBlock->size)]->nextBlock = buddyBlock;
	}
	// Insert the remaining larger block into the free list
	if (manager->freeLists[bsize] == NULL) {
		manager->freeLists[bsize] = newBlock;
	} else {
		// If there's already a block in the free list of the same size, insert at the end
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
				return currentBlock; // Found the block in the free list
			}
			currentBlock = currentBlock->nextBlock;
		}
	}

	return NULL; // Block not found in any free list
}


void freeMemory(MemoryManagerADT manager, void *ptr) {
	BlockADT block = findBlock(manager, ptr);
	if (block == NULL || block->isFree) {
		return;
	}

	block->isFree = TRUE;
	int bsize = log2(block->size);
	BlockADT buddyBlock;

	// Try to coalesce with buddy blocks
	while (block->size < manager->size) {
		size_t offset = block->startAddress - manager->startAddress;
		size_t buddyAddress = (offset ^ block->size) + (size_t)manager->startAddress;
		buddyBlock = findBlock(manager, (void *)buddyAddress);

		if (buddyBlock && buddyBlock->isFree && buddyBlock->size == block->size) {
			// Remove buddy block from free list
			BlockADT *freeList = &manager->freeLists[bsize];
			while (*freeList != NULL && *freeList != buddyBlock) {
				freeList = &(*freeList)->nextBlock;
			}
			if (*freeList == buddyBlock) {
				*freeList = buddyBlock->nextBlock;
			}

			// Merge blocks
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

	// Insert the block back into the free list
	if(buddyBlock != NULL)
		block->nextBlock = manager->freeLists[bsize];
	manager->freeLists[bsize] = block;
}




int main(){
	void * memoryForManager = malloc(1000);
	void * memToAllocate = malloc(1000);
	MemoryManagerADT memoryManager = createMemoryManager(memoryForManager, memToAllocate);
	initManager(memoryManager);
	void * buddyAlloc = allocMemory(memoryManager, 45);
	void * bAlloc2 = allocMemory(memoryManager, 17);
	void * bAlloc3 = allocMemory(memoryManager, 345612);
	int * arr = (int *)bAlloc2;
	for(int i = 0; i < 3; i++){
		arr[i] = i;
	}
	void * bAlloc4 = allocMemory(memoryManager, 1);
	void * bAlloc5 = allocMemory(memoryManager, 234);

	freeMemory(memoryManager, arr);
	freeMemory(memoryManager, buddyAlloc);
}