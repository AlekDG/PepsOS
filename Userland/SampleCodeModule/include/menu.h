#ifndef MENU_H
#define MENU_H

typedef struct Border
{
      int thickness;
      int length;
      int height;
} Border;

typedef struct Option
{
      int isClicked;
      int isHovered;
      Border borde;
      char texto[50];
} Option;

typedef struct OptionMenu
{
      Option *options[6];
} OptionMenu;

void drawOptionMenuArray(OptionMenu *optionMenu);
void drawMenu();
void drawSnakeEndingScreen(unsigned int puntos);
void drawSnakeInterface(unsigned int bg);
void setPoints(int points, unsigned int bg);
void draw2pSnake(unsigned int bg);
void draw2pEnding(int player);

#endif
