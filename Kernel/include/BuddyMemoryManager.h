//
// Created by jo on 03/06/24.
//

#ifndef C_UNIT_TESTING_EXAMPLE_MAIN_BUDDYMEMORYMANAGER_H
#define C_UNIT_TESTING_EXAMPLE_MAIN_BUDDYMEMORYMANAGER_H
#define TRUE 1
#define FALSE 0
typedef struct BlockCDT * BlockADT;
typedef struct MemoryManagerCDT * MemoryManagerADT;
MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory);
#endif // C_UNIT_TESTING_EXAMPLE_MAIN_BUDDYMEMORYMANAGER_H
