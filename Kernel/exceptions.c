#include <video.h>
#include <lib.h>
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE 6

static void zero_division();
static void invalid_opcode();
void exceptionDispatcher(int exception,uint64_t regs) {
	clear();
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	else if (exception == INVALID_OPCODE)
		invalid_opcode();
	saveRegsBuffer(regs);
	printRegs();
	drawStringFormatted("Press any key to continue\n",WHITE,BLACK,2);
	//int flag = 0;
	
	//while(!flag){
//		char c = getKbChar();
//		if(c!=0){
//			flag=1;
//		}
//	}
}

static void zero_division() {
	drawStringFormatted("FATAL ERROR: Could not divide by 0\n",WHITE,BLACK,3);
}

static void invalid_opcode() {
	drawStringFormatted("FATAL ERROR: Invalid opcode\n",WHITE,BLACK,3);
}