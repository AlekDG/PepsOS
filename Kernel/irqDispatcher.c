#include <time.h>
#include <stdint.h>
#include <keyboard.h>
#include <video.h>
#include <kernel.h>
#include <sound.h>
#include <clock.h>

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

	/*
	if(keyIsPressed)
		drawLetterBuffered();*/
}

int int_80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
	switch (rdi)
	{
	case 1:
		// sys_read
		break;
	case 2:
		// sys_write
		break;
	case 3:
		return hours();
		break;
	case 4:
		// regRead
		break;
	case 5:
		paintScreen(rsi);
		break;
	case 6:
		drawRectangleCentered(rsi, rdx, rcx, r8, r9);
		break;
	case 7:
		drawRectangle(rsi, rdx, rcx, r8, r9);
		break;
	case 8:
		drawCircle(rsi, rdx, rcx, r8);
		break;
	case 9:
		drawFace(rsi, rdx, rcx);
		break;
	case 10:
		return getKbChar();
		break;
	case 11:
		timer_wait(rsi);
		break;
	case 12:
		return ticks_elapsed();
		break;
	case 13:
		setSize(rsi);
		break;
	case 14:
		drawLetterBuffered();
		break;
	case 15:
		beep(rsi, rdx);
		break;
	case 16:
		return getFullWidth();
		break;
	case 17:
		return getFullHeight();
		break;
	case 18:
		deleteLetterBuffered();
		break;
	case 19:
		drawLetterFromChar(rsi);
		break;
	case 20:
		return getSize();
		break;
	case 21:
		setXBuffer(rsi);
		break;
	case 22:
		setYBuffer(rsi);
		break;
	case 23:
		return getXBuffer();
		break;
	case 24:
		return getYBuffer();
		break;
	case 25:
		return getBGColor();
		break;
	case 26:
		return getFGColor();
		break;
	case 27:
		return getFGColorPointer();
		break;
	case 28:
		return getXBufferPointer();
		break;
	case 29:
		return getYBufferPointer();
		break;
	case 30:
		setFGColor(rsi);
		break;
	case 31:
		setBGColor(rsi);
		break;
	case 32:
		putpixelResizable(rsi, rdx, rcx, r8);
		break;
	case 33:
		return seconds();
		break;
	case 34:
		paintScreen(rsi);
		break;
	default:
		return 0;
	}
	return 0;
}
