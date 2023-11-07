#include <stdint.h>
#include <stdint.h>

#ifndef SNAKE_H
#define SNAKE_H

#define SQUARE_SIZE 20
#define FACE_RADIUS 15
#define WHITE 0xFFFFFF
#define CARAMEL_BROWN 0x613613
#define RED 0xFF0000
#define MAX_LENGTH 500
#define PURPLE 0x8F00FF
#define INTERFACE_LENGTH 100
#define INITIAL_LENGTH 6
#define SNAKE_COLOR 0x00A86B
#define EAT_FREQ 400
#define END_FREQ 700

enum Movement
{
      DOWN,
      UP,
      RIGHT,
      LEFT
};

struct Point
{
      uint32_t x;
      uint32_t y;
};

struct Snake
{
      struct Point body[MAX_LENGTH];
      uint32_t length;
      uint32_t color;
      enum Movement mov;
};

int checkSnakeEatFace(int offsetX, int offsetY, uint32_t headX, uint32_t headY, int gameMode, uint32_t faceX, uint32_t faceY); // gameMode = 1 para 2 jugadores, cualquier otro para 1 jugador
void drawRandomFace();
void start_game();
void eat();
void drawSnakeHead(uint32_t x, uint32_t y, struct Snake *snake);
void moveSnake(uint8_t value, struct Snake *snake);
uint8_t checkSelfCollision(uint32_t x, uint32_t y, struct Snake *snake);
void initializeSnake(struct Snake *snake, uint16_t startingX, uint16_t startingY, uint32_t snakeColor); // startingY debe ser mayor a 100 para que funcione adecuadamente
void redrawSnake(struct Snake *snake);
int updateSnake(struct Snake *snake, uint32_t mapWidth, uint32_t mapHeight, uint8_t *flagWall, uint8_t *flagSnake, int gameMode, uint32_t faceX, uint32_t faceY);
void gameInput();
int checkSnakeEatRevamped(uint32_t headX, uint32_t headY, int gameMode, uint32_t faceX, uint32_t faceY);

#endif
