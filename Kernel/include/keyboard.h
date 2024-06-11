#ifndef KEYBOARD
#define KEYBOARD

#include <stdint.h>
void keyAct(void);
void regCheckSave(uint64_t*regs);
void gameplay_on(void);
void gameplay_off(void);

#endif