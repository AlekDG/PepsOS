#ifndef VIDEO_H
#define VIDEO_H
#include <stdint.h>

#define PEPSIBLUE 0x0000FF
#define BLACK 0x000000
#define BLUE 0x0000AA
#define GREEN 0x00AA00
#define CYAN 0x00AAAA
#define RED 0xAA0000
#define MAGENTA 0xAA00AA
#define BROWN 0xAA5500
#define LIGHT_GRAY 0xAAAAAA
#define DARK_GRAY 0x555555
#define BRIGHT_BLUE 0x5555FF
#define BRIGHT_GREEN 0x55FF55
#define BRIGHT_CYAN 0x55FFFF
#define BRIGHT_RED 0xFF5555
#define BRIGHT_MAGENTA 0xFF55FF
#define YELLOW 0xFFFF55
#define WHITE 0xFFFFFF

void initialState();
void putpixel(uint32_t hexColor, uint32_t x, uint32_t y);
void paintScreen(uint32_t hexColor);
void drawRectangle(uint32_t hexColor, uint32_t xStart, uint32_t yStart, uint32_t width, uint32_t height);
void drawLetter(uint8_t letter[13][8], uint32_t hexColor, uint32_t x_offset, uint32_t y_offset);
void putpixelResizable(uint32_t hexColor, uint32_t x, uint32_t y, int size);
void drawLetterResizable(uint8_t letter[13][8], uint32_t x_offset, uint32_t y_offset);
void write(char string[], uint32_t x_offset, uint32_t y_offset);
void drawCircle(uint32_t hexColor, int centerX, int centerY, int radius);
void drawFace(uint32_t startingX, uint32_t startingY, uint32_t size);
int getSize();
uint32_t getFGColor();
uint32_t getBGColor();
uint32_t getXBuffer();
uint32_t getYBuffer();
uint32_t getFullHeight();
uint32_t getFullWidth();
void setSize(unsigned int size);
void setFGColor(uint32_t hexColor);
void setBGColor(uint32_t hexColor);
void setXBuffer(uint16_t xPos);
void setYBuffer(uint16_t yPos);
void deleteLetterBuffered();
void highlightBuffer();
void drawMenu();
void drawRectangleCentered(uint32_t hexColor, uint32_t xStart, uint32_t yStart, uint32_t mywidth, uint32_t myheight);
void drawLetterFromChar(char letter);
void printInteger(int num);
void printIntFormatted(int num, uint32_t fg, uint32_t bg, uint32_t size);
void drawStringFormatted(char str[], uint32_t fg, uint32_t bg, uint32_t size);
void drawLetterFormatted(char letter, uint32_t fg, uint32_t bg, uint32_t size);

#endif


