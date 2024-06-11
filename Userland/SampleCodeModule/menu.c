#include <stdint.h>
#include <menu.h>
#include <consola.h>
#include <pepsiman.h>
#include <UserSyscalls.h>
#include <time.h>
#include <snake.h>
#include <snake2.h>
static int p;

void aa(){
	call_sleep(p);
	call_exit();
}

void prueba(){
	int pids[2];
	for(int i = 0; i < 10; i++){
		p = i;
		char* ar[] = {"aa"};
		call_createBackgroundProcess(aa,0,ar,i%5);
	}

	int as = 1;
	while (as)
	{
		as = call_waitKids();
	}
	
	call_exit();
}


void hoverOverOption(Option *option) { option->isHovered = 1; }
void deactivateHover(Option *option) { option->isHovered = 0; }

// USAR ESTA FUNCION PARA PASAR A LA SIGUIENTE FUNCION DEL MENU
void hoverOverNextoption(OptionMenu *optionMenu)
{

	// voy a iterar sobre el array de opciones para ver cual de todas esta ON
	// solo habra una a la vez
	for (int i = 0; i < 6; i++)
	{
		if (optionMenu->options[i]->isHovered)
		{
			deactivateHover(optionMenu->options[i]);
			if (i == 5)
			{ // estoy en la ultima, salto a la primera
				hoverOverOption(optionMenu->options[0]);
			}
			else
			{
				hoverOverOption(optionMenu->options[i + 1]); // resalto la primera
			}
			break;
		}
	}

	call_setXBuffer(50);
	call_setYBuffer(50);
	drawOptionMenuArray(optionMenu); // tengo que volver a renderizar todo el menu
}

void hoverOverPreviousOption(OptionMenu *optionMenu)
{
	for (int i = 0; i < 6; i++)
	{
		if (optionMenu->options[i]->isHovered)
		{
			deactivateHover(optionMenu->options[i]);
			if (i == 0)
			{ // estoy en la primera, salto a la ultima
				hoverOverOption(optionMenu->options[5]);
			}
			else
			{
				hoverOverOption(optionMenu->options[i - 1]); // resalto la primera
			}
			break;
		}
	}
	call_setXBuffer(50);
	call_setYBuffer(50);
	drawOptionMenuArray(optionMenu); // tengo que volver a renderizar todo el menu
}

// ESTO NO SE SI SE VA A USAR
void clickOption(Option *option) { option->isClicked = 1; }
void unclickOption(Option *option) { option->isClicked = 0; }

// todo

// drawTriangle(){funcion que dibuja la flechita de las opciones}

void drawOption(Option option, uint32_t *globalFGColor, uint32_t *globalXPos, uint32_t *globalYPos)
{
	call_setBGColor(WHITE);
	call_setFGColor(PEPSIBLUE); // por seguridad
	int globalSize = call_getSize();
	uint32_t globalBGColor = call_getBGColor();
	uint32_t currentFG = call_getFGColor();
	int letterWidth = globalSize * 8;
	int gap = 3 * globalSize;
	int borderHeight = option.borde.height * globalSize;
	int borderLength = option.borde.length * letterWidth;
	int borderThickness = option.borde.thickness * globalSize;

	// si esta hovereada tengo que cambiar el color del borde
	if (option.isHovered)
	{
		*globalFGColor = RED; // hacer un define de colores pls xD
	}

	// aca adentro asumo que tengo el buffer setedo correctamente, y voy a dibujar
	// exactamente donde el recuadro
	call_drawRectangle(*globalFGColor, *globalXPos, *globalYPos,
				 borderLength + (2 * (borderThickness + gap)),
				 borderHeight + (2 * (borderThickness + gap)));

	// empiezo a dibujar con offset de +thickness
	call_drawRectangle(globalBGColor, *globalXPos + borderThickness, *globalYPos + borderThickness,
				 borderLength + (2 * gap),
				 borderHeight + (2 * gap));

	// desplazo el buffer de la esquina sup. izq.
	// al centro del recuadro, para escribir la opcion
	*globalYPos += (borderThickness + gap);
	*globalXPos += (borderThickness + gap); // dejo espacio de 3 letras
	// vuelvo a cambiar el color porque el texto deberia ir normal
	*globalFGColor = currentFG;

	// buffer seteado
	for (int i = 0; option.texto[i] != 0; i++)
	{
		call_drawLetterFromChar(option.texto[i]);
	}

	// dejo el buffer al ppio del primer recuadro, a la izquierda
	*globalXPos -= (borderLength + borderThickness + gap);
	*globalYPos += (gap + borderThickness);
}


void drawOptionMenuArray(OptionMenu *optionMenu)
{

	call_setXBuffer(50);
	call_setYBuffer(50);

	// dibujo todas las opciones
	for (int i = 0; i < 6; i++)
	{
		drawOption(*(optionMenu->options[i]), call_getFGColorPointer(), call_getXBufferPointer(), call_getYBufferPointer());
		call_setYBuffer(call_getYBuffer() + (2 * (call_getSize() * (optionMenu->options[0]->borde.height)))); // globalYPos+= 2*(globalSize * (optionMenu->options[0]->borde.height));
	}
}




void drawMenu()
{
	int width = call_getWidth();
	int height = call_getHeight();
	call_drawRectangle(BLACK, 0, 0, width, height);
	drawPepsiman(width - 500, 0, 5);
	drawPepsos(width - 460, 500, 3);

	// la primer opcion empieza hovereada
	Option snake1p = {0, 1, {4, 14, 13}, "Jugar Snake 1p"};
	Option snake2p = {0, 0, {4, 14, 13}, "Jugar Snake 2p"};
	Option hora = {0, 0, {4, 13, 13}, "Imprimir Hora"};
	Option registros = {0, 0, {4, 18, 13}, "Imprimir Registros"};
	Option consola = {0, 0, {4, 14, 13}, "Correr Consola"};
	Option restart = {0, 0, {4, 14, 13}, "Reiniciar Menu"};

	OptionMenu optionMenu = {{&snake1p, &snake2p, &hora, &registros, &consola, &restart}};

	/*
	modo de uso de consola de comandos:
	-los comandos seran un solo string
	-el espacio se utiliza para introducir el comando (SI LLEGAMOS A NECESITAR ARGUMENTOS HAY QUE HACER AJUSTE)
	*/

	drawOptionMenuArray(&optionMenu);
	while (1)
	{
	char letter = call_pipe_read(STDIN);
	switch (letter)
	{
	case '\n':
		if (optionMenu.options[0]->isHovered)
		{ // jugar snake 1p
		call_begin_gameplay();
			char* argv[] = {"snake1"}; 
			call_createForegroundProcess(start_game,0,argv,4);
			call_end_gameplay();
			return;
			}
			else if (optionMenu.options[1]->isHovered)
			{
				call_begin_gameplay();
				char* argv[] = {"snake2"}; 
				call_createForegroundProcess(start_gameTwo,0,argv,4);
				call_end_gameplay();
				return;
			}
			else if (optionMenu.options[2]->isHovered)
			{
				char aux[8];
				timeToStr(aux);
				call_drawStringFormatted(aux, WHITE, BLACK, 5);
				// PEDIR TECLA
				call_setYBuffer(call_getYBuffer() + 130);
				call_setXBuffer(50);
				call_drawStringFormatted("->PRESIONE CUALQUIER TECLA PARA CONTINUAR",
								 WHITE, BLACK, 1);
				call_pipe_read(STDIN);
    			return;
			}
			else if (optionMenu.options[3]->isHovered)
			{
				call_drawRectangle(BLACK, 0, 0, call_getWidth(), call_getHeight());
				call_regRead();
				call_pipe_read(STDIN);
    			return;
			}
			else if (optionMenu.options[4]->isHovered)
			{ // consola.isHovered
				char* argv[] = {"consola"}; 
				call_createForegroundProcess(runConsole,0,argv,4);
				//runConsole(&optionMenu);
				return;
			}
			else if (optionMenu.options[5]->isHovered) // restart menu
			{
				return;
			}
			break;
		case 17:
		case 'w':
			hoverOverPreviousOption(&optionMenu);
			break;
		case 20:
		case 's':
			hoverOverNextoption(&optionMenu);
			break;
		default:
			break;
		}
	}
}

/*===========================
Menu para el snake
=============================*/

void drawEndingContext()
{
	int half_width = call_getWidth() / 2;
	int half_height = call_getHeight() / 2;
	int size = 25;

	drawFancyMenu(half_width - size * 45 / 2, half_height - size * 25 / 2, size);
	call_setFGColor(DARK_GRAY);
	call_setBGColor(LIGHT_GRAY);
	call_drawRectangle(LIGHT_GRAY, half_width - 125, half_height - 90, 300, 75);
	call_drawRectangle(LIGHT_GRAY, half_width - 290, half_height + 15, 300, 75);
	call_drawRectangle(LIGHT_GRAY, half_width + 50, half_height + 15, 300, 75);

	call_setXBuffer(half_width - 275);
	call_setYBuffer(half_height + 35);
	char menuText[] = "ESC => Menu";
	for (int j = 0; menuText[j] != 0; j++)
	{
		call_drawLetterFromChar(menuText[j]);
	}

	call_setXBuffer(half_width + 65);
	call_setYBuffer(half_height + 35);
	char replayText[] = "ENTER => Replay";
	for (int k = 0; replayText[k] != 0; k++)
	{
		call_drawLetterFromChar(replayText[k]);
	}
}

void drawSnakeEndingScreen(unsigned int puntos)
{
	drawEndingContext();
	int half_width = call_getWidth() / 2;
	int half_height = call_getHeight() / 2;
	call_setXBuffer(half_width - 100);
	call_setYBuffer(half_height - 65);
	call_setFGColor(DARK_GRAY);
	call_setBGColor(LIGHT_GRAY);
	char puntosText[] = "Puntuacion: ";

	call_setSize(2);
	for (int i = 0; puntosText[i] != 0; i++)
	{
		call_drawLetterFromChar(puntosText[i]);
	}
	call_printInteger(puntos);
}

void draw2pEnding(int player)
{
	drawEndingContext();
	int half_width = call_getWidth() / 2;
	int half_height = call_getHeight() / 2;
	call_setXBuffer(half_width - 90);
	call_setYBuffer(half_height - 65);
	call_setFGColor(DARK_GRAY);
	call_setBGColor(LIGHT_GRAY);
	char player1wins[] = "Player 1 wins!";
	char player2wins[] = "Player 2 wins!";
	call_setSize(2);
	if (player == 1)
	{
		for (int i = 0; player1wins[i] != 0; i++)
		{
			call_drawLetterFromChar(player1wins[i]);
		}
	}
	else if (player == 2)
	{
		for (int i = 0; player2wins[i] != 0; i++)
		{
			call_drawLetterFromChar(player2wins[i]);
		}
	}
}

void drawSnakeInterface(unsigned int bg)
{
	int fullwidth = call_getWidth();
	int interfaceheight = 100;
	call_drawRectangle(WHITE, 0, 0, fullwidth, interfaceheight);
	call_drawRectangle(bg, 13, 13, fullwidth - 26, interfaceheight - 13 * 2);

	call_setXBuffer(39);
	call_setYBuffer(35);
	call_drawStringFormatted("Puntuacion: ", WHITE, bg, 2);

	call_printIntFormatted(0, WHITE, bg, 2);
}

void setPoints(int points, unsigned int bg)
{
	call_drawRectangle(bg, 231, 35, 80, 26);
	call_setXBuffer(231);
	call_printIntFormatted(points, WHITE, bg, 2);
}

void draw2pSnake(unsigned int bg)
{
	int fullwidth = call_getWidth();
	int interfaceheight = 100;
	call_drawRectangle(WHITE, 0, 0, fullwidth, interfaceheight);
	call_drawRectangle(bg, 13, 13, fullwidth - 26, interfaceheight - 13 * 2);

	call_setXBuffer(39);
	call_setYBuffer(35);
	call_drawStringFormatted("Playing 2p snake     P1 = WHITE   P2 = PURPLE", WHITE, bg, 2);
}
