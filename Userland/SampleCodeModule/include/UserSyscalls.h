#ifndef USERSYSCALLS
#define USERSYSCALLS
#include "../../../Shared/include/ps.h"
#include <consola.h>
#include <menu.h>
#include <stdint.h>

#define STDIN 0
#define KILL_SIGNAL 3
#define EOF 4

// Allocate, free, and check memory
void *call_malloc(int size);
void call_free(void *toFree);
void call_mem_state(unsigned long long int *freeMemory,
                    unsigned long long int *totalMemory,
                    unsigned long long int *allocatedMemory);
// Semaphores
int call_sem_open(int value, char *sem_name);
int call_sem_close(char *sem_name);
uint64_t call_sem_create(uint64_t starting_value, char *sem_name);
uint64_t call_sem_post(int sem);
uint64_t call_sem_wait(int sem);
int call_get_sem_by_name(char *sem_name);
// pipes
int call_pipe_open(char *pipe_name);
int call_pipe_close(int pipe);
int call_pipe_write(int pipe, char *string);
char call_pipe_read(int pipe);
// Prints the Registries
void call_regRead(void);
// Paints the entire screen a single color in hexadecimal
void call_paintScreen(uint32_t color);
// Draw shapes of specified dimentions
void call_drawRectangle(uint32_t color, uint32_t offset_x, uint32_t offset_y,
                        uint32_t width, uint32_t height);
void call_drawCircle(uint32_t color, uint32_t offset_x, uint32_t offset_y,
                     uint32_t radius);
void call_drawFace(uint32_t offset_x, uint32_t offset_y, uint32_t size);
// Get a char from the keyboard. Non-blocking
uint8_t call_getChar();
// Wait a specified number of ticks (1/18th of a second)
void call_wait(int ticks);
// Recieve how many ticks have passed
int call_ticks(void);
// Set font size
void call_setSize(unsigned int size);
// Play a beep of a certain frequency for a certain amount of ticks
void call_beep(uint32_t freq, uint8_t tix);
// Recieve Window Height and Width
uint32_t call_getWidth();
uint32_t call_getHeight();
// Delete and draw individual letters with the video driver
void call_deleteLetterBuffered();
void call_drawLetterFromChar(char letter);
// Get font size
unsigned int call_getSize();
// Set and get the Position for the video driver to print characters
void call_setXBuffer(uint16_t xPos);
void call_setYBuffer(uint16_t yPos);
uint32_t call_getXBuffer();
uint32_t call_getYBuffer();
uint32_t *call_getFGColorPointer();
uint32_t *call_getXBufferPointer();
uint32_t *call_getYBufferPointer();
void call_new_line();
// Get and set the color of the text and background
uint32_t call_getBGColor();
uint32_t call_getFGColor();
void call_setFGColor(uint32_t hexcolor);
void call_setBGColor(uint32_t hexcolor);
// Draw an individual pixel of a certain color at a certain position
void call_putpixelResizable(uint32_t hexColor, uint32_t x, uint32_t y,
                            int size);
// Recieve how many seconds have passed
unsigned int call_seconds(void);
// Cursed
void call_begin_gameplay(void);
void call_end_gameplay(void);
// Print various data types
void call_printInteger(int num);
void call_print_long_long_int(unsigned long long int num);
void call_printHex(int num);
void call_printIntFormatted(int num, uint32_t fg, uint32_t bg, uint32_t size);
void call_drawStringFormatted(char str[], uint32_t fg, uint32_t bg,
                              uint32_t size);
void call_drawLetterFormatted(char letter, uint32_t fg, uint32_t bg,
                              uint32_t size);
// Manually trigger exceptions
void call_excepDivZero(void);
void call_excepInvalidOp(void);
// Recieve information regarding date and time. According to RTC
unsigned int call_seconds();
unsigned int call_minutes();
unsigned int call_hours();
unsigned int call_day();
unsigned int call_month();
unsigned int call_year();
// scheduling
int call_createBackgroundProcess(void process(), int argc, char *argv[],
                                 int priority, int *rw_pipes);
int call_createForegroundProcess(void process(), int argc, char *argv[],
                                 int priority, int *rw_pipes);
void call_exit();
int call_kill(int pid);
void call_yield();
int call_changePriority(int pid, int priority);
processInfo *call_ps(int *processCount);
int call_block(int pid);
int call_unblock(int pid);
int call_getPid();
void call_printProcessInfo(processInfo *process);
void call_sleep(int numberOfTicks);
int call_waitKids();
#endif
