#include <time.h>
#include <stdint.h>
#include <keyboard.h>
#include <video.h>
#include <lib.h>
#include <sound.h>
#include <memMan.h>
#include <scheduler.h>

static void int_20();
static void int_21();
static int int_80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

typedef void (*InterruptHandler)(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

void irqDispatcher(uint64_t irq, uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	InterruptHandler interruption[256] = {0};
	interruption[0] = &int_20;
	interruption[1] = &int_21;
	interruption[96] = (InterruptHandler)int_80;

	if (irq >= 0 && irq < 256 && interruption[irq] != 0)
	{
		InterruptHandler handler = interruption[irq];
		handler(rdi, rsi, rdx, rcx, r8, r9);
		return;
	}
}

void int_20()
{
	timer_handler();
}

void int_21()
{
	keyAct();
}

int int_80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	switch (rdi)
	{
	case 1:
		printRegs();
		break;
	case 2:
		paintScreen(rsi);
		break;
	case 3:
		drawRectangle(rsi, rdx, rcx, r8, r9);
		break;
	case 4:
		drawCircle(rsi, rdx, rcx, r8);
		break;
	case 5:
		drawFace(rsi, rdx, rcx);
		break;
	case 6:
		return getKbChar();
		break;
	case 7:
		timer_wait(rsi);
		break;
	case 8:
		return ticks_elapsed();
		break;
	case 9:
		setSize(rsi);
		break;
	case 10:
		beep(rsi, rdx);
		break;
	case 11:
		return getFullWidth();
		break;
	case 12:
		return getFullHeight();
		break;
	case 13:
		deleteLetterBuffered();
		break;
	case 14:
		drawLetterFromChar(rsi);
		break;
	case 15:
		return getSize();
		break;
	case 16:
		setXBuffer(rsi);
		break;
	case 17:
		setYBuffer(rsi);
		break;
	case 18:
		return getXBuffer();
		break;
	case 19:
		return getYBuffer();
		break;
	case 20:
		return getBGColor();
		break;
	case 21:
		return getFGColor();
		break;
	case 22:
		return getFGColorPointer();
		break;
	case 23:
		return getXBufferPointer();
		break;
	case 24:
		return getYBufferPointer();
		break;
	case 25:
		setFGColor(rsi);
		break;
	case 26:
		setBGColor(rsi);
		break;
	case 27:
		putpixelResizable(rsi, rdx, rcx, r8);
		break;
	case 28:
		return seconds();
		break;
	case 29:
		paintScreen(rsi);
		break;
	case 30:
		printInteger(rsi);
		break;
	case 31:
		drawLetterFormatted(rsi, rdx, rcx, r8);
		break;
	case 32:
		drawStringFormatted(rsi, rdx, rcx, r8);
		break;
	case 33:
		printIntFormatted(rsi, rdx, rcx, r8);
		break;
	case 34:
		return minutes();
		break;
	case 35:
		return hours();
		break;
	case 36:
		return day();
		break;
	case 37:
		return month();
		break;
	case 38:
		return year();
		break;
	case 39:
		return allocMemory(rsi, rdx);
		break;
	case 40:
		return createProcess(rsi, rdx, rcx);
		break;
	case 41:
		return getPid();
		break;
	case 42:
		 return createBackgroundProcess(rsi, rdx, rcx);
		 break;
	case 43:
		return createForegroundProcess(rsi, rdx, rcx);
		break;
	case 44:
		exit();
		break;
	default:
		return 0;
	}
	return 0;
}
