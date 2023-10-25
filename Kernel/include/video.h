#ifndef VIDEO_H
#define VIDEO_H
#include <stdint.h>

/*
*----->DIBUJA UN PIXEL EN PANTALLA<------
*return: void
*
*/
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

#endif


