#include <snake.h>
#include <stdint.h>
#include <clock.h>
#include <time.h>

struct Point
{
      uint8_t x;
      uint8_t y;
};

struct Snake
{
      struct Point body[20]; // Habria q ver cual seria la mayor cantidad de partecitas para la vibora
      // body[0] seria la "cabeza", que es lo que marca el recorrido
      uint8_t length;
};

struct Snake snake;

// Habria que hacer un menu inicial para que cuando el jugador pierda pueda volver a jugar apretando ahi

void start_game()
{
      buildMap();
      drawRandomFace();
      initializeSnake(&snake);

      uint8_t flagWall = 0;
      uint8_t flagSnake = 0;
      uint8_t checkFlag = 1; // Para el loop en el que vemos si la serpiente se choco consigo misma o no

      int lastTick = ticks_elapsed();
      int currentTick;
      unsigned int delay = 6;

      int mapWidth = 20 * 40;
      int mapHeight = 20 * 40;

      uint32_t white = 0xFFFFFF;
      uint32_t green = 0x15F03;
      uint32_t lightGreen = 0xC0E3AF;

      while (flagWall == 0 && flagSnake == 0)
      {
            currentTick = ticks_elapsed();

            // Chequeamos si se choca con la pared
            if (snake.body[0].x >= mapWidth || snake.body[0].x < 0 ||
                snake.body[0].y >= mapHeight || snake.body[0].y < 0)
            {
                  flagWall = 1;
            }

            // Chequeamos si se choca consigo misma
            for (int i = 1; i < snake.length && checkFlag; i++)
            {
                  if (snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y)
                  {
                        flagSnake = 1;
                        checkFlag = 0;
                  }
            }

            if (currentTick - lastTick >= delay)
            {
                  // moveSnake(&snake)

                  // Este seria el codigo para el caso en el que se toque la tecla de abajo:

                  uint16_t lastTailX = snake.body[0].x;
                  uint16_t lastTailY = snake.body[0].y;

                  for (int i = 0; i < snake.length - 1; i++)
                  {
                        snake.body[i] = snake.body[i + 1];
                  }
                  snake.body[snake.length - 1].y += 40;

                  drawRectangle(white, snake.body[snake.length - 1].x, snake.body[snake.length - 1].y, 40, 40); // Pintamos la "cabeza nueva"

                  uint32_t boardX = lastTailX / 40; // Dejamos el mapa tal cual estaba originalmente, antes del paso de la snake x ahi
                  uint32_t boardY = lastTailY / 40;
                  uint32_t originalColor = ((boardX + boardY) % 2 != 0) ? green : lightGreen;
                  drawRectangle(originalColor, lastTailX, lastTailY, 40, 40); // Changed to drawRectangle

                  lastTick = currentTick;
            }
      }
      // aca lanzamos un mensajito en pantalla diciendo "perdiste. comiste x caras"
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
