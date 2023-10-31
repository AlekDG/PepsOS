#include <video.h>

void drawConsole(){
	drawRectangle(LIGHT_GRAY, 0, 2*(getFullHeight()/3), getFullWidth(), getFullHeight()/3);
}
void deleteConsole(){
	drawRectangle(BLACK, 0, 2*(getFullHeight()/3), getFullWidth(), getFullHeight()/3);
}

int compareStrings(char * s1, char * s2){
	while(*s1 != 0 && *s2!=0){
		char first = *s1;
		char last = *s2;
		if(first > last){
			return 1;
		}
		else if(first < last){
			return -1;
		}
		s1++;
		s2++;	//sigo comparando
	}
	if(*s1 != 0 && *s2==0){return 1;}
	if(*s1 == 0 && *s2!=0){return -1;}
	return 0;	//ambos son iguales
}

void printHelp(){
	char text[] = "HELP!";
	for(int i=0; text[i]!=0; i++){
		drawLetterFromChar(text[i]);
	}
}
void printSaracatunga(){
	char text[] = "SARACATUNGA";
	for(int i=0; text[i]!=0; i++){
		drawLetterFromChar(text[i]);
	}
}
void enlargeFontSize(){
    setSize(getSize()+1);
}
void decreaseFontSize(){
    setSize(getSize()+1);
}
void greedIsGood(){
	char text[] = "+500g";
	for(int i=0; text[i]!=0; i++){
		drawLetterFromChar(text[i]);
	}
}

void interpretCommand(char command[]){
	char enlargefontsize[] = "increasefont";
	char reducefontsize[] = "reducefont";
	char printsaracatunga[] = "printsaracatunga";
	char greedisgood[] = "greedisgood";
	char printhelp[] = "help";

	if(compareStrings(command, enlargefontsize)==0){
		enlargeFontSize();
		printInteger(getSize());
	}
	if(compareStrings(command, printsaracatunga)==0){
		printSaracatunga();
	}
	if(compareStrings(command, greedisgood)==0){
		greedIsGood();
	}
	if(compareStrings(command, printhelp)==0){
		printHelp();
	}
	if(compareStrings(command, reducefontsize)==0){
		if(getSize()>1){
			decreaseFontSize();
		}
		else{
			setSize(1);
		}
	}
}
void runConsole(OptionMenu * optionMenu){
	setXBuffer(0);
	setYBuffer(2*(getFullHeight()/3) + 13*getSize());

	char internalBuffer[50] ={0};	//tamaño maximo de 50 chars
	int bufferSize = 0;
	int consoleRunning = 1;	//flag de corte de ejecucion
	char currentLetter;

	while(consoleRunning){
		currentLetter = drawLetterBuffered();

		switch(currentLetter){
			case '\n':
				interpretCommand(internalBuffer);
				break;
			case 0:
				break;					//omite teclas no asignadas
			case '1':
				consoleRunning = 0;		//cierra la terminal
				for(int i=0; i<bufferSize;i++){
					internalBuffer[i]=0;		//limpia buffer antes de terminar
					deleteLetterBuffered();
				}
				bufferSize = 0;
				deleteConsole();
				drawRectangle(BLACK, 0, 0, getFullWidth(), getFullHeight());
				drawOptionMenuArray(optionMenu);
				break;
			case '\b':						//borrado de caracter
				if(bufferSize<=0 || getXBuffer()==0){
					bufferSize=0;
					break;
				}
				deleteLetterBuffered();
				internalBuffer[bufferSize--] = 0;	//borra el ultimo caracter y reduce el tamaño del buffer
				break;
			default:
				if(bufferSize>=50){
					break;
				}
				internalBuffer[bufferSize++] = currentLetter;	//se guarda la letra escrita 
				break;
		}		
	}
}
