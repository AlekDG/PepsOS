#include <stdint.h>

#ifndef USERSYSCALLS 
#define USERSYSCALLS

#define INCODE 0
#define OUTCODE 1
#define ERRCODE 2

void call_read(char *strn,int size,int fd);
void call_write(char *strn,int size,int fd);
void call_time(char* strn);
void call_regRead(void);
void call_clearScrean(uint32_t color);
void call_drawRectangle(uint32_t color,uint32_t offset_x,uint32_t offset_y,uint32_t width,uint32_t height);
void call_drawCenteredRectangle(uint32_t color,uint32_t offset_x,uint32_t offset_y,uint32_t width,uint32_t height);
void call_drawCircle(uint32_t color,uint32_t offset_x,uint32_t offset_y,uint32_t radius);
void call_drawFace(uint32_t offset_x,uint32_t offset_y,uint32_t size);
int call_bufferPosition();
uint8_t call_getChar(uint32_t position);
void call_wait(int ticks);
int call_ticks(void);
void call_setTextSize(int size);
char call_drawBufferedChar(void);
void call_beep(uint32_t freq,uint8_t tix);

#endif