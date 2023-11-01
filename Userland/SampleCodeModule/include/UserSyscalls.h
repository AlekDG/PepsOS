#include <stdint.h>

#ifndef USERSYSCALLS
#define USERSYSCALLS

#define INCODE 0
#define OUTCODE 1
#define ERRCODE 2

void call_read(char *strn, int size, int fd);
void call_write(char *strn, int size, int fd);
void call_time(char *strn);
void call_regRead(void);
void call_paintScreen(uint32_t color);
void call_drawRectangle(uint32_t color, uint32_t offset_x, uint32_t offset_y, uint32_t width, uint32_t height);
void call_drawCenteredRectangle(uint32_t color, uint32_t offset_x, uint32_t offset_y, uint32_t width, uint32_t height);
void call_drawCircle(uint32_t color, uint32_t offset_x, uint32_t offset_y, uint32_t radius);
void call_drawFace(uint32_t offset_x, uint32_t offset_y, uint32_t size);
int call_bufferPosition();
uint8_t call_getChar();
void call_wait(int ticks);
int call_ticks(void);
void call_setSize(unsigned int size);
char call_drawBufferedChar(void);
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
uint32_t call_getBGColor();
uint32_t call_getFGColor();
uint32_t *call_getFGColorPointer();
uint32_t *call_getXBufferPointer();
uint32_t *call_getYBufferPointer();
void call_setFGColor(uint32_t hexcolor);
void call_setBGColor(uint32_t hexcolor);
void call_putpixelResizable(uint32_t hexColor, uint32_t x, uint32_t y, int size);
#endif
