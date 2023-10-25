#include <snake.h>
#include <stdint.h>
#include <clock.h>
#include <time.h>
#include <video.h>

#define SQUARE_SIZE 40
#define DELAY_TICKS 3 // Ajustar esto segun la dificultad
#define FACE_RADIUS 15
#define WHITE 0xFFFFFF
#define CARAMEL_BROWN 0x613613
#define RED 0xFF0000
#define MAX_LENGTH 70

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
      struct Point body[MAX_LENGTH]; // Habria q ver cual seria la mayor cantidad de partecitas para la vibora
      // body[0] seria la "cabeza", que es lo que marca el recorrido
      uint32_t length;
};

struct Snake snake;

enum Movement mov = DOWN;

uint32_t faceStartingX;
uint32_t faceStartingY;

// Habria que hacer un menu inicial para que cuando el jugador pierda pueda volver a jugar apretando ahi

void start_game()
{
      buildMap();
      drawRandomFace();
      initializeSnake(&snake);

      uint8_t flagWall = 0;
      uint8_t flagSnake = 0;

      uint32_t lastTick = ticks_elapsed();
      uint32_t currentTick;
      unsigned int delay = DELAY_TICKS;

      uint32_t mapWidth = getFullWidth();
      uint32_t mapHeight = getFullHeight();

      struct Point head;

      while (flagWall == 0 && flagSnake == 0)
      {
            currentTick = ticks_elapsed();

            head.x = snake.body[0].x;
            head.y = snake.body[0].y;

            switch (mov)
            {
            case DOWN:
                  head.y += SQUARE_SIZE;
                  checkSnakeEatFace(SQUARE_SIZE, 0, head.x, head.y);
                  break;
            case UP:
                  head.y -= SQUARE_SIZE;
                  checkSnakeEatFace(SQUARE_SIZE, 0, head.x, head.y);
                  break;
            case RIGHT:
                  head.x += SQUARE_SIZE;
                  checkSnakeEatFace(0, SQUARE_SIZE, head.x, head.y);
                  break;
            case LEFT:
                  head.x -= SQUARE_SIZE;
                  checkSnakeEatFace(0, SQUARE_SIZE, head.x, head.y);
                  break;
            default:
                  break;
            }

            // Wall collision check
            if (head.x >= mapWidth || head.x < 0 ||
                head.y >= mapHeight || head.y < 0)
            {
                  flagWall = 1;
            }

            for (int i = 1; i < snake.length && !flagSnake; i++)
            {
                  if (head.x == snake.body[i].x && head.y == snake.body[i].y)
                  {
                        flagSnake = 1;
                  }
            }
            /*
                        if ((head.x >= faceStartingX && head.x <= faceStartingX + SQUARE_SIZE) &&
                            (head.y >= faceStartingY && head.y <= faceStartingY + SQUARE_SIZE))
                        {
                              eat();
                              drawRectangle(CARAMEL_BROWN, faceStartingX, faceStartingY, SQUARE_SIZE, SQUARE_SIZE);
                              drawRandomFace();
                        } // Arreglar los casos limite de esto */

            if (currentTick - lastTick >= delay)
            {
                  drawRectangle(CARAMEL_BROWN, snake.body[snake.length - 1].x, snake.body[snake.length - 1].y, SQUARE_SIZE, SQUARE_SIZE);

                  for (int i = snake.length - 1; i > 0; i--)
                  {
                        snake.body[i] = snake.body[i - 1];
                  }

                  snake.body[0] = head;

                  for (int i = 1; i < snake.length; i++)
                  {
                        drawRectangle(WHITE, snake.body[i].x, snake.body[i].y, SQUARE_SIZE, SQUARE_SIZE);
                  } // Si no hacemos esto se bugean las caras, ver si encontramos otra solucion

                  drawSnakeHead(head.x, head.y);

                  lastTick = currentTick;
            }
      }
      // Aca debieramos poner un mensajito tipo perdiste, la cantidad de caras comidas final fue...
}

void checkSnakeEatFace(int offsetX, int offsetY, uint32_t headX, uint32_t headY)
{
      uint8_t eaten = 0;
      for (int i = 0; i <= offsetX && !eaten; i++)
      {
            for (int j = 0; j <= offsetY && !eaten; j++)
            {
                  if ((headX + i >= faceStartingX && headX + i <= faceStartingX + SQUARE_SIZE) &&
                      (headY + j >= faceStartingY && headY + j <= faceStartingY + SQUARE_SIZE))
                  {
                        eat();
                        drawRectangle(CARAMEL_BROWN, faceStartingX, faceStartingY, SQUARE_SIZE, SQUARE_SIZE);
                        drawRandomFace();
                        eaten = 1;
                  }
            }
      }
}

void moveSnake(uint8_t value)
{
      switch (value)
      {
      case 0:
            if (mov != UP)
            {
                  mov = DOWN;
            }
            break;
      case 1:
            if (mov != DOWN)
            {
                  mov = UP;
            }
            break;
      case 2:
            if (mov != LEFT)
            {
                  mov = RIGHT;
            }
            break;
      case 3:
            if (mov != RIGHT)
            {
                  mov = LEFT;
            }
            break;
      }
}

void drawSnakeHead(uint32_t x, uint32_t y)
{
      drawRectangle(WHITE, x, y, SQUARE_SIZE, SQUARE_SIZE);

      uint32_t eyeX = x + (3 * SQUARE_SIZE / 4);
      uint32_t eyeY = y + (SQUARE_SIZE / 4);
      uint32_t eyeRadius = SQUARE_SIZE / 8;
      drawCircle(RED, eyeX, eyeY, eyeRadius);
}

void initializeSnake(struct Snake *snake)
{
      snake->length = 6;

      for (int i = 0; i < snake->length; i++)
      {
            snake->body[i].x = 50 + (snake->length - 1 - i) * SQUARE_SIZE;
            snake->body[i].y = 50;

            if (i == 0)
            {
                  drawSnakeHead(snake->body[i].x, snake->body[i].y);
            }
            else
            {
                  drawRectangle(0xFFFFFF, snake->body[i].x, snake->body[i].y, SQUARE_SIZE, SQUARE_SIZE);
            }
      }
}
uint32_t seed;

uint32_t rand_()
{
      seed = (seed * 1664525 + 1013904223) & 0xFFFFFFFF; // Linear Congruential Generator
      return seed;
}

uint32_t getRandom(uint32_t min, uint32_t max)
{
      return (rand_() % (max - min + 1)) + min;
}

uint8_t checkCollision(uint32_t x, uint32_t y)
{
      for (int i = 0; i < snake.length; i++)
      {
            if (x <= snake.body[i].x + SQUARE_SIZE && x >= snake.body[i].x &&
                y <= snake.body[i].y + SQUARE_SIZE && y >= snake.body[i].y)
            {
                  return 1; // Collision detected
            }
      }
      return 0; // No collision
}

void drawRandomFace()
{
      static uint8_t initialized = 0;

      if (!initialized)
      {
            seed = seconds();
            initialized = 1;
      }

      uint32_t minX = FACE_RADIUS;
      uint32_t minY = FACE_RADIUS;
      uint32_t maxX = getFullWidth() - FACE_RADIUS;
      uint32_t maxY = getFullHeight() - FACE_RADIUS;

      uint8_t collision = 1;
      do
      {
            faceStartingX = minX + getRandom(50, 500) % (maxX - minX + 1);
            faceStartingY = minY + getRandom(50, 500) % (maxY - minY + 1);

            collision = checkCollision(faceStartingX, faceStartingY);
      } while (collision);

      drawFace(faceStartingX, faceStartingY, SQUARE_SIZE);
}

void eat()
{
      struct Point newTail;
      newTail.x = snake.body[snake.length - 1].x;
      newTail.y = snake.body[snake.length - 1].y;

      snake.body[snake.length] = newTail;
      snake.length++;
};
