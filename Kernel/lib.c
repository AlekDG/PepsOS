#include <lib.h>
#include <video.h>

#define REG_AMOUNT 10

void *memset(void *destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char *dst = (char *)destination;

	while (length--)
		dst[length] = chr;

	return destination;
}

void *memcpy(void *destination, const void *source, uint64_t length)
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
		uint32_t *d = (uint32_t *)destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t *d = (uint8_t *)destination;
		const uint8_t *s = (const uint8_t *)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

#define BUFFER_SIZE 10

char kbBuffer[BUFFER_SIZE] = {0};
uint8_t bufferIndx = 0;
uint8_t keysPressed = 0;

void addToBuffer(char c)
{
	if (bufferIndx >= BUFFER_SIZE)
		bufferIndx = 0;
	kbBuffer[bufferIndx++] = c;
}

void keyPressed(bool state)
{
	if (state)
		keysPressed++;
	else
		keysPressed--;
}

bool keyIsPressed(void)
{
	return keysPressed >= 0;
}

char getKbChar(void)
{
	if (bufferIndx <= 0)
		return 0;
	return kbBuffer[--bufferIndx];
}

regStruct regBuffer = {0};

void saveRegsBuffer(uint64_t regs)
{
	uint64_t *cast = (uint64_t *)regs;
	regBuffer.r15 = *cast++;
	regBuffer.r14 = *cast++;
	regBuffer.r13 = *cast++;
	regBuffer.r12 = *cast++;
	regBuffer.r11 = *cast++;
	regBuffer.r10 = *cast++;
	regBuffer.r9 = *cast++;
	regBuffer.r8 = *cast++;
	regBuffer.rsi = *cast++;
	regBuffer.rdi = *cast++;
	regBuffer.rbp = *cast++;
	regBuffer.rdx = *cast++;
	regBuffer.rcx = *cast++;
	regBuffer.rbx = *cast++;
	regBuffer.rax = *cast++;
	regBuffer.rip = *cast++;
	regBuffer.cs = *cast++;
	regBuffer.rflags = *cast++;
	regBuffer.rsp = *cast++;
	regBuffer.ss = *cast;
}

void printRegs(void)
{
	uint32_t currentfg = getFGColor();
	uint32_t currentbg = getBGColor();
	setFGColor(WHITE);
	setBGColor(BLACK);
	setXBuffer(0);
	setYBuffer(45);
	drawStringDef("RIP = 0x");
	printHex(regBuffer.rip);
	newLine();
	drawStringDef("RAX = 0x");
	printHex(regBuffer.rax);
	newLine();
	drawStringDef("RBX = 0x");
	printHex(regBuffer.rbx);
	newLine();
	drawStringDef("RCX = 0x");
	printHex(regBuffer.rcx);
	newLine();
	drawStringDef("RDX = 0x");
	printHex(regBuffer.rdx);
	newLine();
	drawStringDef("RSP = 0x");
	printHex(regBuffer.rsp);
	newLine();
	drawStringDef("RBP = 0x");
	printHex(regBuffer.rbp);
	newLine();
	drawStringDef("RSI = 0x");
	printHex(regBuffer.rsi);
	newLine();
	drawStringDef("RDI = 0x");
	printHex(regBuffer.rdi);
	newLine();
	drawStringDef("R8 = 0x");
	printHex(regBuffer.r8);
	newLine();
	drawStringDef("R9 = 0x");
	printHex(regBuffer.r9);
	newLine();
	drawStringDef("R10 = 0x");
	printHex(regBuffer.r10);
	newLine();
	drawStringDef("R11 = 0x");
	printHex(regBuffer.r11);
	newLine();
	drawStringDef("R12 = 0x");
	printHex(regBuffer.r12);
	newLine();
	drawStringDef("R13 = 0x");
	printHex(regBuffer.r13);
	newLine();
	drawStringDef("R14 = 0x");
	printHex(regBuffer.r14);
	newLine();
	drawStringDef("R15 = 0x");
	printHex(regBuffer.r15);
	newLine();
	drawStringDef("CS = 0x");
	printHex(regBuffer.cs);
	newLine();
	drawStringDef("RFLAGS = 0x");
	printHex(regBuffer.rflags);
	newLine();
	drawStringDef("SS = 0x");
	printHex(regBuffer.ss);
	newLine();
	setFGColor(currentfg);
	setBGColor(currentbg);
}

#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 0x04
#define DAY 0x07
#define MONTH 0x08
#define YEAR 0x09

unsigned char clock(unsigned char mode); // esta en clock.asm

unsigned int decode(unsigned char time)
{
	return (time >> 4) * 10 + (time & 0x0F);
}

unsigned int seconds()
{
	return decode(clock(SECONDS));
}

unsigned int minutes()
{
	return decode(clock(MINUTES));
}

unsigned int hours()
{
	return decode(clock(HOURS));
}

unsigned int day()
{
	return decode(clock(DAY));
}

unsigned int month()
{
	return decode(clock(MONTH));
}

unsigned int year()
{
	return decode(clock(YEAR));
}