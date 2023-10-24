#include <snake.h>
#include <stdint.h>
#include <clock.h>

struct Point
{
      uint8_t x;
      uint8_t y;
};

struct Snake
{
      struct Point body[20]; // Habria q ver cual seria la mayor cantidad de partecitas para la vibora
      uint8_t length;
};

struct Snake snake;

void start_game()
{
      buildMap();
      drawRandomFace();
      initializeSnake(&snake);
}

void initializeSnake(struct Snake *snake)
{
      snake->length = 6;
      uint32_t white = 0xFFFFFF; // Hex code for white
      uint32_t squareSize = 40;  // Size of each square for the snake

      for (int i = 0; i < snake->length; i++)
      {
            snake->body[i].x = 50 + i * (squareSize); // inicializo el x, despues hay q modificarlo cada '..' tiempo y cada vez q se mueve
            snake->body[i].y = 50;                    // lo mismo para y

            drawRectangle(white, snake->body[i].x, snake->body[i].y, squareSize, squareSize);
      }
}

uint32_t seed; // Declare seed as a global variable

uint32_t rand_()
{
      seed = (seed * 1664525 + 1013904223) & 0xFFFFFFFF; // Linear Congruential Generator
      return seed;
}

uint32_t getRandom(uint32_t min, uint32_t max)
{
      return (rand_() % (max - min + 1)) + min;
}

void drawRandomFace()
{
      static int initialized = 0;

      if (!initialized)
      {
            seed = seconds();
            initialized = 1;
      }

      int mapSize = 20;
      int squareSize = 40;
      int faceRadius = 35; // Radio de la cara

      // Calcula el rango de valores posibles para el centro de la cara,
      // considerando que no se debe superponer con los bordes del mapa.
      int minX = faceRadius;
      int minY = faceRadius;
      int maxX = mapSize * squareSize - faceRadius;
      int maxY = mapSize * squareSize - faceRadius;

      int centerX = minX + getRandom(50, 500) % (maxX - minX + 1);
      int centerY = minY + getRandom(50, 500) % (maxY - minY + 1);

      // int centerX, centerY;

      /*int overlap;

      do
      {
            overlap = 0;

            centerX = minX + getRandom(50, 500) % (maxX - minX + 1);
            centerY = minY + getRandom(50, 500) % (maxY - minY + 1);

            for (int i = 0; i < snake.length; i++)
            {
                  int snakeX = snake.body[i].x + squareSize / 2;
                  int snakeY = snake.body[i].y + squareSize / 2;

                  int distance = (centerX - snakeX) * (centerX - snakeX) + (centerY - snakeY) * (centerY - snakeY);

                  // Aca el chequeo de si la cara se superpone con la snake
            }
      } while (overlap);*/

      drawFace(centerX, centerY);
}

void eat()
{
}
