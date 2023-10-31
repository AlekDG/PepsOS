#include <time.h>
#include <stdint.h>
#include <keyboard.h>
#include <video.h>
#include <kernel.h>
#include <sound.h>
#include <clock.h>

static void int_20();
static int int_80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_21();
			break;
	}
	return;
}

void int_20() {
	timer_handler();
}

void int_21(){
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
		sys_write((char *)rsi, rdx, rcx);
		break;
	case 2:
		sys_read((char *)rsi, rdx, rcx);
		break;
	case 3:
		hours();
		break;
	case 4:
		// regRead
		break;
	case 5:
		paintScreen(rsi);
		break;
	case 6:
		drawRectangle(rsi, rdx, rcx, r8, r9);
		break;
	case 7:
		drawRectangleCentered(rsi, rdx, rcx, r8, r9);
		break;
	case 8:
		drawCircle(rsi, rdx, rcx, r8);
		break;
	case 9:
		drawFace(rsi, rdx, rcx);
		break;
	case 10:
		// getChar
		break;
	case 11:
		timer_wait(rsi);
		break;
	case 12:
		ticks_elapsed();
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
	default:
		return 0;
	}
	return 0;
}
