#ifndef USERSYSCALLS
#define USERSYSCALLS
#include "../../../Shared/include/ps.h"
#include <consola.h>
#include <menu.h>
#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define KILL_SIGNAL 3
#define EOF 4

void call_system_read(char *output, int length);
void call_system_write(char *string);
void *call_malloc(int size);
void call_free(void *toFree);
void call_mem_state(unsigned long long int *freeMemory,
                    unsigned long long int *totalMemory,
                    unsigned long long int *allocatedMemory);
int call_sem_open(int value, char *sem_name);
int call_sem_close(char *sem_name);
uint64_t call_sem_create(uint64_t starting_value, char *sem_name);
uint64_t call_sem_post(int sem);
uint64_t call_sem_wait(int sem);
int call_get_sem_by_name(char *sem_name);
int call_pipe_open(char *pipe_name);
int call_pipe_close(int pipe);
int call_pipe_write(int pipe, char *string);
char call_pipe_read(int pipe);
void call_regRead(void);
void call_paintScreen(uint32_t color);
void call_drawRectangle(uint32_t color, uint32_t offset_x, uint32_t offset_y,
                        uint32_t width, uint32_t height);
void call_drawCircle(uint32_t color, uint32_t offset_x, uint32_t offset_y,
                     uint32_t radius);
void call_drawFace(uint32_t offset_x, uint32_t offset_y, uint32_t size);
uint8_t call_getChar();
void call_wait(int ticks);
int call_ticks(void);
void call_setSize(unsigned int size);
void call_beep(uint32_t freq, uint8_t tix);
uint32_t call_getWidth();
uint32_t call_getHeight();
void call_deleteLetterBuffered();
void call_drawLetterFromChar(char letter);
unsigned int call_getSize();
void call_setXBuffer(uint16_t xPos);
void call_setYBuffer(uint16_t yPos);
uint32_t call_getXBuffer();
uint32_t call_getYBuffer();
uint32_t *call_getFGColorPointer();
uint32_t *call_getXBufferPointer();
uint32_t *call_getYBufferPointer();
void call_new_line();
void call_set_color(uint32_t fg, uint32_t bg);
void call_reset_color();
void call_clear();
uint32_t call_getBGColor();
uint32_t call_getFGColor();
void call_setFGColor(uint32_t hexcolor);
void call_setBGColor(uint32_t hexcolor);
void call_putpixelResizable(uint32_t hexColor, uint32_t x, uint32_t y,
                            int size);
unsigned int call_seconds(void);
void call_begin_gameplay(void);
void call_end_gameplay(void);
void call_printInteger(int num);
void call_print_long_long_int(unsigned long long int num);
void call_printHex(int num);
void call_printIntFormatted(int num, uint32_t fg, uint32_t bg, uint32_t size);
void call_drawStringFormatted(char str[], uint32_t fg, uint32_t bg,
                              uint32_t size);
void call_drawLetterFormatted(char letter, uint32_t fg, uint32_t bg,
                              uint32_t size);
void call_excepDivZero(void);
void call_excepInvalidOp(void);
unsigned int call_seconds();
unsigned int call_minutes();
unsigned int call_hours();
unsigned int call_day();
unsigned int call_month();
unsigned int call_year();
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
void call_createProcessesWithPipe(void process1(),void process2(), int *argcs, char **argvs[],
                                  int **pipes);
#endif
