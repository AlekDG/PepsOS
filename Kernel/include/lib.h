#ifndef LIB_H
#define LIB_H
#include <stdbool.h>
#include <stdint.h>

#include "../../Shared/include/ps.h"

#define NULL 0
#define FALSE 0
#define TRUE 1

#define POWER_OF_TWO_MAX_EXPONENT 32

typedef struct regStruct {
  uint64_t rax, rbx, rcx, rdx;
  uint64_t rsi, rdi, rbp, rsp;
  uint64_t r8, r9, r10, r11;
  uint64_t r12, r13, r14, r15;
  uint64_t rip, cs, rflags, ss;
} regStruct;

void saveRegsBuffer(uint64_t regs);
void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);
char *cpuVendor(char *result);

// Auxiliary Assembly functions

void userBuild(void);
uint8_t inb(uint16_t in1);
void outb(uint16_t out1, uint8_t in1);

// keyboard support:

uint8_t getKey(void);
void addToBuffer(char c);
char getKbChar(void);
void keyPressed(bool state);
bool keyIsPressed(void);

// clock functionality:

unsigned int seconds();
unsigned int minutes();
unsigned int hours();
unsigned int year();
unsigned int day();
unsigned int month();
void timeToStr(char *dest);
void dateToStr(char *dest);
void getDate(char *dest);

// For printing the registries:
void printRegs();

// For Entering and exiting critical region:
void enter_region(uint64_t *lock_address, uint64_t sem_indx);
void leave_region(uint64_t *lock_address);

// General purpose functions:
unsigned int string_length(const char *str);
void string_copy(char *from, char *to);
int same_string(char *s1, char *s2);
int fast_log2(int X);

//For printing process information
void printProcessInfo(processInfo* process);
#endif