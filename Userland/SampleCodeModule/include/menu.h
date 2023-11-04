#ifndef MENU_H
#define MENU_H

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
      Option *options[5];
} OptionMenu;

void drawOptionMenuArray(OptionMenu *optionMenu);
void drawMenu();
void drawSnakeEndingScreen(unsigned int puntos);
void drawSnakeInterface(unsigned int bg);
void setPoints(int points, unsigned int bg);
void draw2pSnake(unsigned int bg);
void draw2pEnding(int player);

#endif
