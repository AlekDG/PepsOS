#include <UserSyscalls.h>
#include <menu.h>
#include <consola.h>

void drawConsole()
{
	call_drawRectangle(LIGHT_GRAY, 0, 500, call_getWidth(), call_getHeight() - 500); // justo al final de pepsiman
	call_setFGColor(DARK_GRAY);
	call_setBGColor(LIGHT_GRAY);
	int currentsize = call_getSize();
	call_setSize(1);
	call_setXBuffer(call_getWidth() - (40 * call_getSize() * 8));
	call_setYBuffer(call_getHeight() - (13 * call_getSize()));
	char texto[] = "Consola PepsOS ltd. all rights reserved.";
	for (int i = 0; texto[i] != 0; i++)
	{
		call_drawLetterFromChar(texto[i]);
	}
	call_setSize(currentsize);
}
void deleteConsole()
{
	call_drawRectangle(BLACK, 0, 500, call_getWidth(), call_getHeight() - 500);
}

int compareStrings(char *s1, char *s2)
{
	while (*s1 != 0 && *s2 != 0)
	{
		char first = *s1;
		char last = *s2;
		if (first > last)
		{
			return 1;
		}
		else if (first < last)
		{
			return -1;
		}
		s1++;
		s2++; // sigo comparando
	}
	if (*s1 != 0 && *s2 == 0)
	{
		return 1;
	}
	if (*s1 == 0 && *s2 != 0)
	{
		return -1;
	}
	return 0; // ambos son iguales
}

void askForAnyletter()
{
	int currentsize = call_getSize();
	call_setSize(1);
	char text[] = "\n\n\n\n->PRESIONE CUALQUIER TECLA PARA CONTINUAR";
	for (int i = 0; text[i] != 0; i++)
	{
		call_drawLetterFromChar(text[i]);
	}
	call_setSize(currentsize);
}

void printHelp()
{
	deleteConsole();
	call_drawRectangle(LIGHT_GRAY, 0, 500, call_getWidth(), call_getHeight() - 500);
	int currentsize = call_getSize();
	call_setSize(2);
	char text[] = "->Testear division por cero: testdivzero\n->Testear invalid op code: testinvalidopcode\n->Aumentar tamanio de fuente: increasefontsize\n->Reducir tamanio de fuente: reducefontsize";
	call_setXBuffer(0);
	call_setYBuffer(2 * (call_getHeight() / 3) + 13 * call_getSize());
	for (int i = 0; text[i] != 0; i++)
	{
		call_drawLetterFromChar(text[i]);
	}
	call_setSize(currentsize);
}
void enlargeFontSize()
{
	call_setSize(call_getSize() + 1);
}
void decreaseFontSize()
{						    // no actualiza el tamño del menu mientras este en la consola
	call_setSize(call_getSize() - 1); // es comportamiento esperado
}
void interpretCommand(char command[])
{
	char enlargefontsize[] = "increasefont";
	char reducefontsize[] = "reducefont";
	char testdivzero[] = "testdivzero";
	char testinvalidopcode[] = "testinvalidopcode";
	char printhelp[] = "help";
	if (compareStrings(command, enlargefontsize) == 0)
	{
		if (call_getSize() < 4)
		{
			enlargeFontSize();
		}
		else
		{
			call_setSize(4);
		}
		askForAnyletter();
		while (1)
		{
			int letter = call_getChar();
			if (letter != 0)
			{
				return;
			}
		}
	}
	else if (compareStrings(command, reducefontsize) == 0)
	{
		if (call_getSize() > 1)
		{
			decreaseFontSize();
		}
		else
		{
			call_setSize(1);
		}
		askForAnyletter();
		while (1)
		{
			int letter = call_getChar();
			if (letter != 0)
			{
				return;
			}
		}
		return;
	}
	else if (compareStrings(command, testdivzero) == 0)
	{
		call_excepDivZero();
		return;
	}
	else if (compareStrings(command, testinvalidopcode) == 0)
	{
		call_excepInvalidOp();
		return;
	}
	else if (compareStrings(command, printhelp) == 0)
	{ // imprime comandos de ayuda
		printHelp();
		askForAnyletter();
		while (1)
		{
			int letter = call_getChar();
			if (letter != 0)
			{
				return;
			}
		}
	}
}
void runConsole()
{
	drawConsole();
	call_setXBuffer(0);
	call_setYBuffer(2 * (call_getHeight() / 3) + 13);

	char internalBuffer[50] = {0}; // tamaño maximo de 50 chars
	int bufferSize = 0;
	int consoleRunning = 1; // flag de corte de ejecucion
	char currentLetter;

	while (consoleRunning)
	{
		currentLetter = call_getChar();
		switch (currentLetter)
		{
		case '\n':
			interpretCommand(internalBuffer);
			drawConsole();
			while (bufferSize > 0)
			{
				internalBuffer[bufferSize - 1] = 0;
				bufferSize--;
			}
			call_setXBuffer(0);
			call_setYBuffer(2 * (call_getHeight() / 3) + 13);
			break;
		case 0: // omite teclas no asignadas
			break;
		case 27: // current letter=esc
			call_exit();
		case '\b': // borrado de caracter
			if (bufferSize <= 0)
			{
				bufferSize = 0;
				break;
			}
			call_deleteLetterBuffered();
			internalBuffer[bufferSize--] = 0; // borra el ultimo caracter y reduce el tamaño del buffer
			break;
		default:
			if (bufferSize >= 50)
			{
				break;
			}
			call_drawLetterFromChar(currentLetter);
			internalBuffer[bufferSize++] = currentLetter; // se guarda la letra escrita
			break;
		}
	}
}
