#include <lib.h>
#include <video.h>

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

#define BUFFER_SIZE 10

char kbBuffer[BUFFER_SIZE] = {0};
uint8_t bufferIndx = 0;
uint8_t keysPressed = 0;

void addToBuffer(char c){
	if(bufferIndx>=BUFFER_SIZE)
		bufferIndx=0;
	kbBuffer[bufferIndx++]=c;
}

void keyPressed(bool state){
	if(state)
		keysPressed++;
	else
		keysPressed--;
}

bool keyIsPressed(void){
	return keysPressed>=0;
}

char getKbChar(void){
	if(bufferIndx<=0)
		return 0;
	return kbBuffer[--bufferIndx];
}

void printRegs(regStruct*toPrint){
	drawStringDef("RIP = 0x");
	printInteger(toPrint->rip);
	drawStringDef("RAX = 0x");
	printInteger(toPrint->rax);
	drawStringDef("RBX = 0x");
	printInteger(toPrint->rbx);
	drawStringDef("RCX = 0x");
	printInteger(toPrint->rcx);
	drawStringDef("RDX = 0x");
	printInteger(toPrint->rdx);
	drawStringDef("RSP = 0x");
	printInteger(toPrint->rsp);
	drawStringDef("RBP = 0x");
	printInteger(toPrint->rbp);
	drawStringDef("RSI = 0x");
	printInteger(toPrint->rsi);
	drawStringDef("RDI = 0x");
	printInteger(toPrint->rdi);
	drawStringDef("R8 = 0x");
	printInteger(toPrint->r8);
	drawStringDef("R9 = 0x");
	printInteger(toPrint->r9);
	drawStringDef("R10 = 0x");
	printInteger(toPrint->r10);
	drawStringDef("R11 = 0x");
	printInteger(toPrint->r11);
	drawStringDef("R12 = 0x");
	printInteger(toPrint->r12);
	drawStringDef("R13 = 0x");
	printInteger(toPrint->r13);
	drawStringDef("R14 = 0x");
	printInteger(toPrint->r14);
	drawStringDef("R15 = 0x");
	printInteger(toPrint->r15);
}