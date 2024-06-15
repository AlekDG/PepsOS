#ifndef IO_H
#define IO_H

#define PEPSIBLUE 0x0000FF
#define BLACK 0x000000
#define BLUE 0x0000AA
#define GREEN 0x00AA00
#define CYAN 0x00AAAA
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

#define EOF 4

void print_error(char* string);
void print_f(char* string);
void print_f_int(int num);
char get_char();
void put_char(char c);


#endif