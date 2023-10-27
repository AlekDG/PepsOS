#include <stdint.h>
#include <stdlib.h>
#include <kernel.h>
#include <video.h> 
#include <font.h>
#include <time.h>

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
const VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x5c00;
uint32_t offSetX = 0;
uint32_t offSetY = 98;

uint16_t height;
uint16_t width;
uint32_t globalFGColor; 
uint32_t globalBGColor;
uint32_t globalSize;
int globalXPos;
int globalYPos;

void initialState(){
	height = VBE_mode_info->height;
	width = VBE_mode_info->width;
	globalFGColor = 0x0000FF;
	globalBGColor = 0xFFFFFF;
	globalSize = 1;
	globalXPos = 0;
	globalYPos = 0;
}


int getSize(){
	return globalSize;
}
uint32_t getFGColor(){
	return globalFGColor;
}
uint32_t getBGColor(){
	return globalBGColor;
}
uint32_t getXBuffer(){
	return globalXPos;
}
uint32_t getYBuffer(){
	return globalYPos;
}
uint32_t getFullHeight(){
	return height;
}
uint32_t getFullWidth(){
	return width;
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
void setXBuffer(uint16_t xPos){
	globalXPos = xPos;
}
void setYBuffer(uint16_t yPos){
	globalYPos = yPos;
}


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

void drawFace(uint32_t startingX, uint32_t startingY, uint32_t faceSize)
{
	drawRectangle(0xFFFF00, startingX, startingY, faceSize, faceSize); // Face

	uint32_t eyeRadius = faceSize / 15;
	uint32_t eyeXOffset = faceSize / 3;
	uint32_t eyeYOffset = faceSize / 3;

	// Draw eyes
	drawCircle(0x000000, startingX + eyeXOffset, startingY + eyeYOffset, eyeRadius);
	drawCircle(0x000000, startingX + 2 * eyeXOffset, startingY + eyeYOffset, eyeRadius);

	// Draw mouth
	for (int x = startingX + eyeXOffset; x <= startingX + 2 * eyeXOffset; x++)
	{
		int mouthY = startingY + 2 * eyeYOffset;
		for (int y = mouthY; y <= mouthY + 2; y++)
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
				putpixelResizable(globalFGColor, j*globalSize+x_offset, i*globalSize+y_offset, globalSize);
			}
			else{
				putpixelResizable(globalBGColor, j*globalSize+x_offset, i*globalSize+y_offset, globalSize);
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
		drawLetterResizable(letterBuffer, x_offset, y_offset);
		x_offset += (8*globalSize);
	}
}


void moveBuffer(){
	if(globalXPos+(globalSize*8) >= width){
		if(globalYPos+(globalSize*13) >= height){
			globalYPos = height;	//estoy al final, no hace nada
		}
		else{
			globalYPos+=(globalSize*13);
			globalXPos=0;
		}
	}
	else{
		globalXPos+=(globalSize*8);
	}
}

void drawLetterBuffered(){
	char letter =getKbChar();
	if(letter==0)
		return;
	uint8_t buffer[13][8] = {0};
	getLetter(letter,buffer);
	drawLetterResizable(buffer, globalXPos, globalYPos);
	moveBuffer();
}

void drawLetterFromChar(char letter){
	if(letter==0)
		return;
	uint8_t buffer[13][8] = {0};
	getLetter(letter,buffer);
	drawLetterResizable(buffer, globalXPos, globalYPos);
	moveBuffer();
}

void backtrackBuffer(){
	//tengo que hacer putpixel del color del bg
	//mover el buffer para atras con 1*size
	if(globalXPos < globalSize*8 ){	
		//estoy al ppio del buffer con x=0
		//o borre el primer caracter y X vale mas que width xq es unsigned
		if(globalYPos<globalSize*13){
			return;			//nada que borrar
		}
		else{
			globalYPos-=globalSize*13;
			globalXPos = ((getFullWidth()*8)/8) - (globalSize*8);
		}
	}
	else{
		globalXPos -= globalSize*8;
	}
}


void deleteLetterBuffered(){
	backtrackBuffer();
	drawRectangle(globalBGColor, globalXPos, globalYPos, globalSize*8, globalSize*13);	//dibujo un rectangulo de color BGColor
}


void highlightBuffer(){
	static int ishighlighted = 0;
	if((*ticks_elapsed)() % 18==0 && !ishighlighted){
		drawRectangle(CYAN, globalXPos, globalYPos, globalSize*8, globalSize*13);	//2 pixeles de ancho y el alto de una letra
		ishighlighted=1;
	}
	else if((*ticks_elapsed)() % 18==0 && ishighlighted){
		drawRectangle(globalBGColor, globalXPos, globalYPos, globalSize*8, globalSize*13);	//2 pixeles de ancho y el alto de una letra
		ishighlighted=0;
	}
}




void hoverOverOption(Option * option){option->isHovered=1;}
void deactivateHover(Option * option){option->isHovered=0;}

//USAR ESTA FUNCION PARA PASAR A LA SIGUIENTE FUNCION DEL MENU
void hoverOverNextoption(OptionMenu * optionMenu){

	//voy a iterar sobre el array de opciones para ver cual de todas esta ON
	//solo habra una a la vez
	for(int i=0; i<4; i++){
		if(optionMenu->options[i].isHovered){
			deactivateHover(&(optionMenu->options[i]));
			if(i==3){   //estoy en la ultima, salto a la primera
				hoverOverOption(&(optionMenu->options[0]));
			}
			else{
				hoverOverOption(&(optionMenu->options[i+1]));  //resalto la primera
			}
			break;
		}
	}
	
	setXBuffer(50);
	setYBuffer(50);
	drawMenu();   //tengo que volver a renderizar todo el menu
}

void hoverOverPreviousOption(OptionMenu * optionMenu){
	for(int i=0; i<4; i++){
		if(optionMenu->options[i].isHovered){
			deactivateHover(&(optionMenu->options[i]));
			if(i==0){   //estoy en la primera, salto a la ultima
				hoverOverOption(&(optionMenu->options[3]));
			}
			else{
				hoverOverOption(&(optionMenu->options[i-1]));  //resalto la primera
				
			}
			break;
		}
	}
	setXBuffer(50);
	setYBuffer(50);
	drawMenu();   //tengo que volver a renderizar todo el menu
	
}



//ESTO NO SE SI SE VA A USAR
void clickOption(Option * option){option->isClicked = 1;}
void unclickOption(Option * option){option->isClicked = 0;}

//todo

//drawTriangle(){funcion que dibuja la flechita de las opciones}

void drawOption(Option option){

	uint32_t currentFG = getFGColor();
	int borderHeight = option.borde.height;
	int borderLength = option.borde.length;
	int borderThickness = option.borde.thickness;
	
	//si esta hovereada tengo que cambiar el color del borde
	if(option.isHovered){
		globalFGColor = RED;     //hacer un define de colores pls xD
	}

	//aca adentro asumo que tengo el buffer setedo correctamente, y voy a dibujar 
	//exactamente donde el recuadro
	drawRectangle(globalFGColor, globalXPos, globalYPos, 
	borderLength*globalSize, borderHeight*globalSize);

	//empiezo a dibujar con offset de +thickness
	drawRectangle(globalBGColor, globalXPos + borderThickness, globalYPos + borderThickness, 
	(borderLength -(2*borderThickness))*globalSize, (borderHeight-(2*borderThickness))*globalSize);
	
	//desplazo el buffer de la esquina sup. izq. 
	//al centro del recuadro, para escribir la opcion
	globalYPos += (borderThickness + borderHeight/2);
	globalXPos += (borderThickness + (globalSize*8*3));	//dejo espacio de 3 letras
	//vuelvo a cambiar el color porque el texto deberia ir normal
	globalFGColor = currentFG;
	
	//buffer seteado
	for(int i=0; option.texto[i]!=0; i++){
		drawLetterFromChar(option.texto[i]);
	}
	
	//dejo el buffer al ppio del primer recuadro, a la izquierda
	globalXPos -= (borderLength + (globalSize*8*3));
	globalYPos += (borderThickness+(borderHeight/2));
}


void drawMenu(){
	//							->strlen			->bordes
	Option registros = {1,0,{4, (18*globalSize*8) + (6*globalSize*2), 13 + 12}, "Imprimir Registros"};
	//la primer opcion empieza hovereada
	
	Option hora = {0,0, {4, (13*globalSize*8) + (6*globalSize*2), 13 + 12}, "Imprimir Hora"};
	Option snake = {0,0, {4, (11*globalSize*8) + (6*globalSize*2), 13 + 12}, "Jugar Snake"};
	Option consola = {0,0, {4, (15*globalSize*8) + (6*globalSize*2), 13 + 12}, "Pedir Registros"};
	
	OptionMenu optionMenu ={{registros, hora, snake, consola}};
	
	setXBuffer(50);
	setYBuffer(50);
	
	//dibujo todas las opciones
	for(int i=0; i<4; i++){
		drawOption(optionMenu.options[i]);
		globalYPos+= 2*(registros.borde.height);
	}


	/*while(1){
		hoverOverNextoption(&optionMenu);

		hoverOverNextoption(&optionMenu);
		hoverOverNextoption(&optionMenu);
		hoverOverNextoption(&optionMenu);
		hoverOverNextoption(&optionMenu);
		hoverOverNextoption(&optionMenu);
		hoverOverNextoption(&optionMenu);
		hoverOverNextoption(&optionMenu);
		hoverOverPreviousOption(&optionMenu);
		hoverOverPreviousOption(&optionMenu);
		hoverOverPreviousOption(&optionMenu);
		hoverOverPreviousOption(&optionMenu);
		hoverOverPreviousOption(&optionMenu);
		hoverOverPreviousOption(&optionMenu);
	}*/
}




