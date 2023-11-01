#include <UserSyscalls.h>
#include <consola.h>
#include <menu.h>

void drawConsole(){
	call_drawRectangle(LIGHT_GRAY, 0, 2*(call_getHeight()/3), call_getWidth(), call_getHeight()/3);
	call_setFGColor(DARK_GRAY);
	call_setBGColor(LIGHT_GRAY);	
	call_setXBuffer(call_getWidth()-40*call_getSize()*8);
	call_setYBuffer(call_getHeight()-13*call_getSize());
	char texto[] = "Consola PepsOS ltd. all rights reserved.";
	for(int i=0; texto[i]!=0; i++){
		call_drawLetterFromChar(texto[i]);
	}
}
void deleteConsole(){
	call_drawRectangle(BLACK, 0, 2*(call_getHeight()/3), call_getWidth(), call_getHeight()/3);
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

void restartConsole(){
	deleteConsole();
	runConsole();
}

void printHelp(){
	deleteConsole();
	drawConsole();
	//char text[] = "->Imprimir saracatunga: printsaracatunga\n->Aumentar tamaño de fuente: increasefont\n->Reducir tamaño de fuente: reducefont\n->Obtener oro: greedisgood\n->PRESIONE CUALQUIER TECLA PARA CONTINUAR";
	char text[] = "help";
	call_setXBuffer(0);
	call_setYBuffer(2*(call_getHeight()/3) + 13*call_getSize());
	for(int i=0; text[i]!=0; i++){
		call_drawLetterFromChar(text[i]);
	}
}
void printSaracatunga(){
	char text[] = "\nSARACATUNGA\n";
	for(int i=0; text[i]!=0; i++){
		call_drawLetterFromChar(text[i]);
	}
}
void enlargeFontSize(){
    call_setSize(call_getSize()+1);
	restartConsole();
}
void decreaseFontSize(){	//no actualiza el tamño del menu mientras este en la consola
    call_setSize(call_getSize()-1);	//es comportamiento esperado
	restartConsole();
}
void greedIsGood(){
	char text[] = "+500g";
	for(int i=0; text[i]!=0; i++){
		call_drawLetterFromChar(text[i]);
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
	}
	if(compareStrings(command, printsaracatunga)==0){
		printSaracatunga();
	}
	if(compareStrings(command, greedisgood)==0){
		greedIsGood();
	}
	if(compareStrings(command, printhelp)==0){
		printHelp();
		char c;
		int keyCaptured = 0;
		/*while(!keyCaptured){
			c=getKbChar();
			if(c){
				keyCaptured=1;
			}
		}
		restartConsole();*/
	}
	if(compareStrings(command, reducefontsize)==0){
		if(call_getSize()>1){
			decreaseFontSize();
		}
		else{
			call_setSize(1);
		}
	}
}
void runConsole(OptionMenu * optionMenu){
	drawConsole();
	call_setXBuffer(0);
	call_setYBuffer(2*(call_getHeight()/3) + 13*call_getSize());

	char internalBuffer[50] ={0};	//tamaño maximo de 50 chars
	int bufferSize = 0;
	int consoleRunning = 1;	//flag de corte de ejecucion
	char currentLetter;

	while(consoleRunning){
		currentLetter = getKbChar();
//drawLetterBuffered()
		switch(currentLetter){
			case '\n':
				interpretCommand(internalBuffer);
				break;
			case 0:
				break;					//omite teclas no asignadas
			case 27:
				consoleRunning = 0;		//cierra la terminal
				for(int i=0; i<bufferSize;i++){
					internalBuffer[i]=0;		//limpia buffer antes de terminar
					call_deleteLetterBuffered();
				}
				bufferSize = 0;
				deleteConsole();
				call_drawRectangle(BLACK, 0, 0, call_getWidth(), call_getHeight());
				drawMenu();
				break;
			case '\b':						//borrado de caracter
				if(bufferSize<=0){
					bufferSize=0;
					break;
				}
				call_deleteLetterBuffered();
				internalBuffer[bufferSize--] = 0;	//borra el ultimo caracter y reduce el tamaño del buffer
				break;
			default:
				if(bufferSize>=50){
					break;
				}
				call_drawLetterFromChar(currentLetter);
				internalBuffer[bufferSize++] = currentLetter;	//se guarda la letra escrita 
				break;
		}
	}
}