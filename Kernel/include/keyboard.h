#ifndef KEYBOARD
#define KEYBOARD

#define EOF 4
#define KILL_SIGNAL 3

#include <stdint.h>
void keyAct(void);
void regCheckSave(uint64_t*regs);
void gameplay_on(void);
void gameplay_off(void);

#endif