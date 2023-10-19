#include <stdint.h>
#include "video.h" 
#include "font.h"

	uint32_t offSetX = 0;
	uint32_t offSetY = 98;

void shiftIndx(){
	if(offSetX<VBE_mode_info->width)
		offSetX += 8;
	else{
		offSetX=0;
		if(offSetY<VBE_mode_info->height)
			offSetY+=13;
		else{
			offSetY=0;
			//TODO: clearScreen();
			}
		}
}

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;
VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x5c00;


int globalFGColor = 0x0000FF;
int globalBGColor = 0xFFFFFF;
int globalSize = 1;


void putpixel(uint32_t hexColor, uint32_t x, uint32_t y){
	uint8_t * framebuffer = (uint8_t * )VBE_mode_info -> framebuffer; //FRAMEBUFFER ES VRAM
	
	uint64_t offset = (x * (VBE_mode_info->bpp/8)) + (y * VBE_mode_info->pitch);
	
	framebuffer[offset] = (hexColor) & 0xFF;  
	framebuffer[offset+1] = (hexColor>>8) & 0xFF;
	framebuffer[offset+2] = (hexColor >> 16) & 0xFF; 
	
	
}

void paintScreen(uint32_t hexColor){
	uint32_t myWidth = (VBE_mode_info->width);
	uint32_t myHeight = (VBE_mode_info->height);

	for(uint32_t x=0; x<myWidth; x++){
		for(uint32_t y=0; y<myHeight; y++){
			putpixel(hexColor, x, y);
		}
	}

	/*
	uint32_t xCenter = myWidth/2;
	uint32_t yCenter = myHeight/2;

	for(uint32_t x=300; x<600; x++){
		for(uint32_t y=300; y<600; y++){
			if((x-xCenter)*(x-xCenter)+(y-yCenter)*(y-yCenter)<900)
				putpixel(hexColor, x, y);
		}
	}*/
}

void drawRectangle(uint32_t hexColor, uint32_t xStart, uint32_t yStart, uint32_t width, uint32_t height){
	uint32_t totalWidth, totalHeight;
	totalWidth = VBE_mode_info->width;
	totalHeight = VBE_mode_info->height;


	for(uint32_t x=xStart; x<xStart+width && x<totalWidth; x++){
		for(uint32_t y=yStart; y<yStart+height && y<totalHeight; y++){
			putpixel(hexColor, x, y);
		}
	}
}

void drawCircle(uint32_t hexColor, int centerX, int centerY, int radius)
{
	for (int y = -radius; y <= radius; y++)
	{
		for (int x = -radius; x <= radius; x++)
		{
			if (x * x + y * y <= radius * radius)
			{
				putpixel(hexColor, centerX + x, centerY + y);
			}
		}
	}
}

void drawFace(int centerX, int centerY)
{
	drawCircle(0xFFFF00, centerX, centerY, 35); // Cara

	drawCircle(0x000000, centerX - 10, centerY - 10, 3);
	drawCircle(0x000000, centerX + 10, centerY - 10, 3);

	// se podria hacer con drawRectangle x ahi qda mejor con eso
	for (int x = centerX - 20; x <= centerX + 20; x++)
	{
		for (int y = centerY + 10; y <= centerY + 12; y++)
		{
			putpixel(0x000000, x, y);
		}
	}
}

void drawLetter(uint8_t letter[13][8], uint32_t hexColor, uint32_t x_offset, uint32_t y_offset){
	
	for(int i=0; i<13; i++){
		for(int j=0; j<8; j++){	//cada elemento de la matriz lo represento con 2 pixeles
			if(letter[12-i][7-j]==1){
				putpixel(hexColor, j+x_offset, i+y_offset);
				//i++;
				//putpixel(hexColor, i+x_offset, j+y_offset);
			}
			else{
				putpixel(0xFFFFFF, j+x_offset, i+y_offset);
				//i++;
				//putpixel(0xFFFFFF, i+x_offset, j+y_offset);
			}
		}
	}
}



void putpixelResizable(uint32_t hexColor, uint32_t x, uint32_t y, int size){
	uint8_t * framebuffer = (uint8_t * )VBE_mode_info -> framebuffer;
	uint64_t offset = (x * (VBE_mode_info->bpp/8)) + (y * VBE_mode_info->pitch);

	uint16_t myWidth = (uint16_t * ) VBE_mode_info->width;
	uint16_t myHeight = (uint16_t * ) VBE_mode_info->height;//todo LIMITAR LA POSIBILIDAD DE DIBUJAR FUERA DE LA PANTALLA
	uint16_t myPitch = (uint16_t * ) VBE_mode_info->pitch;

	//dibujo un cuadrado de tamaño size*size ---> cuando size=1 es equivalente a hacer putpixel
	for(int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			uint64_t offset = ((x+j) * (VBE_mode_info->bpp/8)) + ((y+i) * myPitch);
			framebuffer[offset] = (hexColor)&0xFF;  
			framebuffer[offset+1] = (hexColor>>8) & 0xFF;
			framebuffer[offset+2] = (hexColor >> 16) & 0xFF; 
			
		}
		//framebuffer+=myPitch;
	}	
}
void drawLetterResizable(uint8_t letter[13][8], uint32_t x_offset, uint32_t y_offset){

	for(int i=0; i<13; i++){
		for(int j=0; j<8; j++){
			if(letter[12-i][7-j]==1){
				putpixelResizable(globalFGColor, j*size+x_offset, i*size+y_offset, globalSize);
			}
			else{
				putpixelResizable(globalBGColor, j*size+x_offset, i*size+y_offset, globalSize);
			}
		}
	}
}

void write(char string[], uint32_t x_offset, uint32_t y_offset){
	uint16_t myHeight = (uint16_t *)VBE_mode_info->height;
	uint16_t myWidth = (uint16_t *)VBE_mode_info->width;

	uint8_t letterBuffer[13][8] = {0};

	for(int i=0; string[i]!=0; i++){
		if(x_offset + (globalSize * 8)>= myWidth){	//si se pasa ya se que la letra no va a entrar
			x_offset = 0;
			y_offset += (13*globalSize);
			if(y_offset>=myHeight){
				return;
			}
		}
		getLetter(string[i], letterBuffer); 
		drawLetterResizable(letterBuffer, globalFGColor, x_offset, y_offset, globalSize);
		x_offset += (8*globalSize);
	}
}

void writeKbInput(){
	uint8_t keyHex = getKey();
	if(keyHex!=0){
		char keyChar = hexToChar(keyHex);
		if(keyChar!=0){
			write(&keyChar,offSetX,offSetY);
			shiftIndx();
		}	
	}
}

void moveCursor(){
	if(globalXPos + 8*globalSize >= width){		//desplazamiento del buffer automatico
		globalXPos = 0;
		if(globalYPos + 13*globalSize >= height){
			globalYPos = 0;
		}
		else{
			globalYPos += 13*globalSize;
		}
	}
	else{
		globalXPos + 8*globalSize;
	}
}

void putBufferedLetter(char letter){
	uint8_t buffer[13][8] = {0};
	getLetter(letter, buffer);
	moveCursor();
	drawLetterResizable(buffer, x_offset, y_offset);
}
void setSize(unsigned int size){
	globalSize = size;
}
void setFGColor(uint32_t hexColor){
	globalFGColor = hexColor;
}
void setBGColor(uint32_t hexColor){
	globalBGColor = hexColor;
}

