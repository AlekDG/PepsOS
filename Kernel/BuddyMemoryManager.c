#include "BuddyMemoryManager.h"
#include "memMan.h"
#include <lib.h>
#define IS_POWER_OF_2(x) (!((x)&((x)-1)))

typedef enum MemoryStatus { EMPTY, PARTIAL, FULL } MemoryStatus;

#define BLOCK 0x1000
typedef struct MemNode *MemNodePtr;

typedef struct MemoryManagerCDT{
    MemNodePtr root;
    uint64_t memAllocated;
    uint64_t totalSize;
    uint64_t freeMemory;
} MemoryManagerCDT;

typedef struct MemNode{
    MemNodePtr right;
    MemNodePtr left;
    uint64_t size;
    MemoryStatus status;
} MemNode;



MemoryManagerADT createMemoryManagerImpl(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);
 void *allocMemoryImpl(MemoryManagerADT manager, size_t size);
 void *genericAllocMemoryRec(MemoryManagerADT memoryManager, MemNodePtr node, unsigned int size);
void setDescendants(MemoryManagerADT memoryManager, MemNodePtr node);
int hasDescendants(MemNodePtr node);
void endRecursiveCall(MemoryManagerADT memoryManager, MemNodePtr node);
void freeMemoryImpl(MemoryManagerADT manager, void *ptr);
 void genericFreeRec(MemoryManagerADT memoryManager, MemNodePtr node, void *memoryToFree);
 uint64_t align(uint64_t size);


 void initMemory(MemoryManagerADT memoryManager, void *const init){
    memoryManager->root = init;
    memoryManager->root->size = USER_MEMORY_SIZE - sizeof(MemNode);
    memoryManager->root->status = EMPTY;
    memoryManager->root->left = NULL;
    memoryManager->root->right = NULL;
}

MemoryManagerADT createMemoryManagerImpl(void *const restrict memoryForMemoryManager, void *const restrict managedMemory){
    MemoryManagerADT memoryManager = (MemoryManagerADT) memoryForMemoryManager;
    initMemory(memoryManager, managedMemory);
    return memoryManager;
}

 uint64_t align(uint64_t size){
    size |= size >> 1;
    size |= size >> 2;
    size |= size >> 4;
    size |= size >> 8;
    size |= size >> 16;
    return size + 1;
}

 void *genericAllocMemory(MemoryManagerADT memoryManager, uint64_t size){
    if(size < BLOCK){
        size = BLOCK;
    }
    else if(size > memoryManager->root->size){
        return NULL;
    }
    if(!IS_POWER_OF_2(size)){
        size = align(size);
    }

    void * allocAttempt = genericAllocMemoryRec(memoryManager, memoryManager->root, size);
    return allocAttempt;
}

 void *genericAllocMemoryRec(MemoryManagerADT memoryManager, MemNodePtr node, unsigned int size){
    if(node == NULL || node->status == FULL){
        return NULL;
    }

    if(node->left != NULL || node->right != NULL){
        void *auxNode = genericAllocMemoryRec(memoryManager, node->left, size);
        if (auxNode == NULL) {
            auxNode = genericAllocMemoryRec(memoryManager, node->right, size);
        }
        endRecursiveCall(memoryManager, node);
        return auxNode;
    }
    else{
        if (size > node->size) {
            return NULL;
        }
        if ((node->size / 2) >= size) {
            setDescendants(memoryManager, node);
            void *auxNode = genericAllocMemoryRec(memoryManager, node->left, size);
            endRecursiveCall(memoryManager, node);
            node->status = PARTIAL;
            return auxNode;
        }
        node->status = FULL;
        memoryManager->memAllocated += node->size + sizeof(MemNode);
        memoryManager->freeMemory -= node->size + sizeof(MemNode);
        return (void *)((uint64_t)node + sizeof(MemNode));
    }
}

 void setDescendants(MemoryManagerADT memoryManager, MemNodePtr node){
    uint64_t descendantSize = ((uint64_t)(node->size) / 2);
    node->left = (MemNodePtr)((uint64_t)node + sizeof(MemNode));

    uint64_t limit = (uint64_t)memoryManager->root + memoryManager->root->size + sizeof(MemNode);

    if ((uint64_t) node->left >= limit) {
        return;
    }
    node->left->size = descendantSize - sizeof(MemNode);
    node->left->status = EMPTY;
    memoryManager->memAllocated += sizeof(MemNode);
    memoryManager->freeMemory -= sizeof(MemNode);

    node->right = (MemNodePtr)((uint64_t)node + descendantSize + sizeof(MemNode));
    if ((uint64_t) node->right >= limit) {
        return;
    }
    node->right->size = descendantSize - sizeof(MemNode);
    node->right->status = EMPTY;

    memoryManager->memAllocated += sizeof(MemNode);
    memoryManager->freeMemory -= sizeof(MemNode);
}

 int hasDescendants(MemNodePtr node){
    return node->left == NULL || node->right == NULL;
}

 void endRecursiveCall(MemoryManagerADT memoryManager, MemNodePtr node){
    if (hasDescendants(node)) {
        node->status = EMPTY;
        memoryManager->freeMemory += node->size;
        memoryManager->memAllocated -= node->size;
        return;
    }
    if (node->left->status == FULL && node->right->status == FULL) {
        node->status = FULL;
    } else if (node->left->status == FULL || node->right->status == FULL
               || node->left->status == PARTIAL || node->right->status == PARTIAL) {
        node->status = PARTIAL;
    } else {
        node->status = EMPTY;
        memoryManager->freeMemory += node->size;
        memoryManager->memAllocated -= node->size;
    }
}

void freeMemoryImpl(MemoryManagerADT MemoryManager, void *memoryToFree){
    genericFreeRec(MemoryManager, MemoryManager->root, memoryToFree);
}

 void genericFreeRec(MemoryManagerADT memoryManager, MemNodePtr node, void *memoryToFree){
    if(node == NULL){
        return;
    }
    if (node->left != NULL || node->right != NULL){
        if (node->right != NULL && (uint64_t) node->right + sizeof(MemNode) > (uint64_t) memoryToFree){
            genericFreeRec(memoryManager, node->left, memoryToFree);
        }
        else{
            genericFreeRec(memoryManager, node->right, memoryToFree);
        }
        endRecursiveCall(memoryManager, node);
        if (node->status == EMPTY){
            node->right = NULL;
            node->left = NULL;
            memoryManager->freeMemory += 2*sizeof(MemNode);
            memoryManager->memAllocated -= 2*sizeof(MemNode);
        }
    }
    else if (node->status == FULL) {
        if ((void *)((uint64_t)node + sizeof(MemNode)) == memoryToFree) {
            node->status = EMPTY;
            memoryManager->memAllocated -= node->size;
            memoryManager->freeMemory += node->size;
        }
    }
    return;
}
void *allocMemoryImpl(MemoryManagerADT manager, size_t size){
    return genericAllocMemory(manager, size);
}

uint64_t getFreeMemoryAmount(MemoryManagerADT MemoryManager){
    return MemoryManager->freeMemory;
}

uint64_t getUsedMemoryAmount(MemoryManagerADT MemoryManager){
    return MemoryManager->memAllocated;
}

void memStateImpl(MemoryManagerADT const restrict memoryManager,
                  unsigned long long int *freeMemory,
                  unsigned long long int *totalMemory,
                  unsigned long long int *allocatedMemory) {
   *freeMemory = getFreeMemoryAmount(memoryManager);
   *allocatedMemory = getUsedMemoryAmount(memoryManager);
   *totalMemory = memoryManager->totalSize;
}

