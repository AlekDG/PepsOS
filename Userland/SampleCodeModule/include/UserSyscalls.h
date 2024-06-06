#include <stdint.h>

#ifndef USERSYSCALLS
#define USERSYSCALLS

//Allocate and free memory
void *call_malloc(int size);
void call_free(void* toFree);
//Prints the Registries
void call_regRead(void);
//Paints the entire screen a single color in hexadecimal
void call_paintScreen(uint32_t color);
//Draw shapes of specified dimentions
void call_drawRectangle(uint32_t color, uint32_t offset_x, uint32_t offset_y, uint32_t width, uint32_t height);
void call_drawCircle(uint32_t color, uint32_t offset_x, uint32_t offset_y, uint32_t radius);
void call_drawFace(uint32_t offset_x, uint32_t offset_y, uint32_t size);
//Get a char from the keyboard
uint8_t call_getChar();
//Wait a specified number of ticks (1/18th of a second)
void call_wait(int ticks);
//Recieve how many ticks have passed
int call_ticks(void);
//Set font size
void call_setSize(unsigned int size);
//Play a beep of a certain frequency for a certain amount of ticks
void call_beep(uint32_t freq, uint8_t tix);
//Recieve Window Height and Width
uint32_t call_getWidth();
uint32_t call_getHeight();
//Delete and draw individual letters with the video driver
void call_deleteLetterBuffered();
void call_drawLetterFromChar(char letter);
//Get font size
unsigned int call_getSize();
//Set and get the Position for the video driver to print characters
void call_setXBuffer(uint16_t xPos);
void call_setYBuffer(uint16_t yPos);
uint32_t call_getXBuffer();
uint32_t call_getYBuffer();
uint32_t *call_getFGColorPointer();
uint32_t *call_getXBufferPointer();
uint32_t *call_getYBufferPointer();
//Get and set the color of the text and background
uint32_t call_getBGColor();
uint32_t call_getFGColor();
void call_setFGColor(uint32_t hexcolor);
void call_setBGColor(uint32_t hexcolor);
//Draw an individual pixel of a certain color at a certain position
void call_putpixelResizable(uint32_t hexColor, uint32_t x, uint32_t y, int size);
//Recieve how many seconds have passed
unsigned int call_seconds(void);
//Print various data types
void call_printInteger(int num);
void call_printIntFormatted(int num, uint32_t fg, uint32_t bg, uint32_t size);
void call_drawStringFormatted(char str[], uint32_t fg, uint32_t bg, uint32_t size);
void call_drawLetterFormatted(char letter, uint32_t fg, uint32_t bg, uint32_t size);
//Manually trigger exceptions
void call_excepDivZero(void);
void call_excepInvalidOp(void);
//Recieve information regarding date and time. According to RTC
unsigned int call_seconds();
unsigned int call_minutes();
unsigned int call_hours();
unsigned int call_day();
unsigned int call_month();
unsigned int call_year();
int call_createProcess(void process(), int argc, char* argv,int priority);
int call_createBackgroundProcess(void process(), int argc, char* argv,int priority);
int call_createForegroundProcess(void process(), int argc, char* argv,int priority);
void call_exit();
#endif
