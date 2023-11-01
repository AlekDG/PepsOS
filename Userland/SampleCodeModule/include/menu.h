#ifndef MENU_H
#define MENU_H

#define LIGHT_GRAY 0xAAAAAA
#define BLACK 0x000000

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


#endif
