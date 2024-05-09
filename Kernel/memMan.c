#include <stdint.h>
#include <stddef.h>
#include <interrupts.h>
#define TOTAL_HEAP_SIZE ( ( size_t ) ( 1024*25 ) )
static uint8_t ucHeap[ TOTAL_HEAP_SIZE ];
static size_t xNextFreeByte = ( size_t ) 0;

void* malloc(int size){
    void *pvReturn = NULL;
    static uint8_t *pucAlignedHeap = NULL;
    static size_t xNextFreeByte = ( size_t ) 0;

	_cli();
	{
		/* Check there is enough room left for the allocation. */
		if( ((xNextFreeByte + size) > xNextFreeByte ))/* Check for overflow. */
		{
			/* Return the next free byte then increment the index past this
			block. */
			pvReturn = pucAlignedHeap + xNextFreeByte;
			xNextFreeByte += size;
		}
	}
	_sti();
	return pvReturn;

}

void free(void* toFree){

}