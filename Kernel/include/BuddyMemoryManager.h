
#ifndef BUDDY_MEMORY_MANAGER_H
#define BUDDY_MEMORY_MANAGER_H
#include <lib.h>

void init_mm(void * base_address, uint64_t mem_amount);
void * mallocBuddy(uint64_t bytes);
void freeBuddy(void * mem);
void buddyState(uint64_t *free,uint64_t *total,uint64_t *alloctd);

#endif
