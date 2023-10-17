#ifndef VIDEO_H
#define VIDEO_H


/*
*----->DIBUJA UN PIXEL EN PANTALLA<------
*return: void
*
*/

void putpixel(uint32_t hexColor, uint32_t x, uint32_t y);
void paintScreen(uint32_t hexColor);
void drawRectangle(uint32_t hexColor, uint32_t xStart, uint32_t yStart, uint32_t width, uint32_t height);
void drawLetter(uint8_t letter[13][8], uint32_t hexColor, uint32_t x_offset, uint32_t y_offset);
void putpixelResizable(uint32_t hexColor, uint32_t x, uint32_t y, int size);
void drawLetterResizable(uint8_t letter[13][8], uint32_t hexColor, uint32_t x_offset, uint32_t y_offset, int size);


#endif


