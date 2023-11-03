#include <stdint.h>
#include <UserSyscalls.h>

#define SQUARE_SIZE 40
#define FACE_RADIUS 15
#define WHITE 0xFFFFFF
#define CARAMEL_BROWN 0x613613
#define RED 0xFF0000
#define MAX_LENGTH 70
#define MAX_PLAYERS 2
#define NULL 0
#define INITIAL_LENGTH 4

uint8_t delayTicks = 3; // Ajustar esto segun la dificultad(Crear una opcion en el menu inicial del juego en el que te pida seleccionar la dificultad).

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
      enum Movement mov;
      uint8_t flagWall;
      uint8_t flagSnake;
};

// struct Snake snake;

struct Snake snakes[MAX_PLAYERS];

uint32_t faceStartingX;
uint32_t faceStartingY;

// Habria que hacer un menu inicial para que cuando el jugador pierda pueda volver a jugar apretando ah
uint8_t playersAmount = 2;

void start_game()
{

      call_paintScreen(CARAMEL_BROWN);
      drawRandomFace();
      if (playersAmount == 2)
      {
            initializeSnake(&snakes[0], &snakes[1]);
      }
      else
      {
            initializeSnake(&snakes[0], NULL);
      }

      uint32_t lastTick = call_ticks();
      uint32_t currentTick;
      unsigned int delay = delayTicks;

      uint32_t mapWidth = call_getWidth();
      uint32_t mapHeight = call_getHeight();

      uint8_t endGame = 0;

      struct Point head;

      while (endGame == 0)
      {
            currentTick = call_ticks();
            for (int i = 0; i < playersAmount; i++)
            {
                  head.x = snakes[i].body[0].x;
                  head.y = snakes[i].body[0].y;

                  snakeCheck(&snakes[i], head);

                  // Chequea si se choco con la pared
                  if (head.x >= mapWidth || head.x < 0 ||
                      head.y >= mapHeight || head.y < 0)
                  {
                        snakes[i].flagWall = 1;
                        endGame = 1;
                  }

                  for (int j = 1; j < snakes[i].length && !snakes[i].flagSnake; j++)
                  {
                        if (head.x == snakes[i].body[j].x && head.y == snakes[i].body[j].y)
                        {
                              snakes[i].flagSnake = 1;
                              endGame = 1;
                        }
                  }

                  if (currentTick - lastTick >= delay)
                  {
                        call_drawRectangle(CARAMEL_BROWN, snakes[i].body[snakes[i].length - 1].x, snakes[i].body[snakes[i].length - 1].y, SQUARE_SIZE, SQUARE_SIZE);

                        for (int j = snakes[i].length - 1; j > 0; j--)
                        {
                              snakes[i].body[j] = snakes[i].body[j - 1];
                        }

                        snakes[i].body[0] = head;

                        for (int j = 1; j < snakes[i].length; j++)
                        {
                              call_drawRectangle(WHITE, snakes[i].body[j].x, snakes[i].body[j].y, SQUARE_SIZE, SQUARE_SIZE);
                        } // Si no hacemos esto se bugean las caras, ver si encontramos otra solucion

                        drawSnakeHead(head.x, head.y);

                        lastTick = currentTick;
                  }
            }
      }
      if (playersAmount == 2)
      {
            if (snakes[0].flagSnake || snakes[0].flagWall == 0)
            {
                  // gano snake[0]
            }
            else
            {
                  // gano snake[1]
            }
      }
      else
      {
            // imprimir el menu que indique cuantas caritas comio...
      }
      // Aca debieramos poner un mensajito tipo perdiste, la cantidad de caras comidas final fue...
}

void snakeCheck(struct Snake *snake, struct Point head)
{
      switch (snake->mov)
      {
      case DOWN:
            head.y += SQUARE_SIZE;
            checkSnakeEatFace(SQUARE_SIZE, 0, head.x, head.y, snake);
            break;
      case UP:
            head.y -= SQUARE_SIZE;
            checkSnakeEatFace(SQUARE_SIZE, 0, head.x, head.y, snake);
            break;
      case RIGHT:
            head.x += SQUARE_SIZE;
            checkSnakeEatFace(0, SQUARE_SIZE, head.x, head.y, snake);
            break;
      case LEFT:
            head.x -= SQUARE_SIZE;
            checkSnakeEatFace(0, SQUARE_SIZE, head.x, head.y, snake);
            break;
      default:
            break;
      }
}

void checkSnakeEatFace(int offsetX, int offsetY, uint32_t headX, uint32_t headY, struct Snake *snake)
{
      uint8_t eaten = 0;
      for (int i = 0; i <= offsetX && !eaten; i++)
      {
            for (int j = 0; j <= offsetY && !eaten; j++)
            {
                  if ((headX + i >= faceStartingX && headX + i <= faceStartingX + SQUARE_SIZE) &&
                      (headY + j >= faceStartingY && headY + j <= faceStartingY + SQUARE_SIZE))
                  {
                        eat(snake);
                        call_drawRectangle(CARAMEL_BROWN, faceStartingX, faceStartingY, SQUARE_SIZE, SQUARE_SIZE);
                        drawRandomFace();
                        eaten = 1;
                  }
            }
      }
}

void moveSnake(struct Snake *snake, uint8_t value)
{
      switch (value)
      {
      case 0:
            if (snake->mov != UP)
            {
                  snake->mov = DOWN;
            }
            break;
      case 1:
            if (snake->mov != DOWN)
            {
                  snake->mov = UP;
            }
            break;
      case 2:
            if (snake->mov != LEFT)
            {
                  snake->mov = RIGHT;
            }
            break;
      case 3:
            if (snake->mov != RIGHT)
            {
                  snake->mov = LEFT;
            }
            break;
      }
}

void drawSnakeHead(uint32_t x, uint32_t y)
{
      call_drawRectangle(WHITE, x, y, SQUARE_SIZE, SQUARE_SIZE);

      uint32_t eyeX = x + (3 * SQUARE_SIZE / 4);
      uint32_t eyeY = y + (SQUARE_SIZE / 4);
      uint32_t eyeRadius = SQUARE_SIZE / 8;
      call_drawCircle(RED, eyeX, eyeY, eyeRadius);
}

void initializeSnake(struct Snake *firstSnake, struct Snake *secondSnake)
{
      firstSnake->length = INITIAL_LENGTH;
      firstSnake->mov = DOWN;
      firstSnake->flagWall = 0;
      firstSnake->flagSnake = 0;

      for (int i = 0; i < firstSnake->length; i++)
      {
            firstSnake->body[i].x = 50 + (firstSnake->length - 1 - i) * SQUARE_SIZE;
            firstSnake->body[i].y = 50;

            if (i == 0)
            {
                  drawSnakeHead(firstSnake->body[i].x, firstSnake->body[i].y);
            }
            else
            {
                  call_drawRectangle(WHITE, firstSnake->body[i].x, firstSnake->body[i].y, SQUARE_SIZE, SQUARE_SIZE);
            }
      }

      if (secondSnake != NULL)
      {
            secondSnake->length = INITIAL_LENGTH;
            secondSnake->mov = DOWN;
            secondSnake->flagWall = 0;
            secondSnake->flagSnake = 0;
            for (int i = 0; i < secondSnake->length; i++)
            {
                  secondSnake->body[i].x = 300 + (secondSnake->length - 1 - i) * SQUARE_SIZE;
                  secondSnake->body[i].y = 50;

                  if (i == 0)
                  {
                        drawSnakeHead(secondSnake->body[i].x, secondSnake->body[i].y);
                  }
                  else
                  {
                        call_drawRectangle(WHITE, secondSnake->body[i].x, secondSnake->body[i].y, SQUARE_SIZE, SQUARE_SIZE);
                  }
            }
      }
}

uint32_t seed;

uint32_t rand_()
{
      seed = (seed * 1664525 + 1013904223) & 0xFFFFFFFF;
      return seed;
}

uint32_t getRandom(uint32_t min, uint32_t max)
{
      return (rand_() % (max - min + 1)) + min;
}

uint8_t checkSelfCollision(uint32_t x, uint32_t y) // chequea que la cara no se dibuje encima de una serpiente
{
      for (int i = 0; i < playersAmount; i++)
      {
            for (int j = 0; j < snakes[i].length; j++)
            {
                  if (x <= snakes[i].body[j].x + SQUARE_SIZE && x >= snakes[i].body[j].x &&
                      y <= snakes[i].body[j].y + SQUARE_SIZE && y >= snakes[i].body[j].y)
                  {
                        return 1;
                  }
            }
      }

      return 0;
}

void drawRandomFace()
{
      static uint8_t initialized = 0;

      if (!initialized)
      {
            seed = call_seconds();
            initialized = 1;
      }

      uint32_t minX = FACE_RADIUS;
      uint32_t minY = FACE_RADIUS;
      uint32_t maxX = call_getWidth() - FACE_RADIUS;
      uint32_t maxY = call_getHeight() - FACE_RADIUS;

      uint8_t collision = 1;
      do
      {
            faceStartingX = minX + getRandom(50, 500) % (maxX - minX + 1);
            faceStartingY = minY + getRandom(50, 500) % (maxY - minY + 1);

            collision = checkSelfCollision(faceStartingX, faceStartingY);
      } while (collision);

      call_drawFace(faceStartingX, faceStartingY, SQUARE_SIZE);
}

void eat(struct Snake *snake)
{
      if (snake->length < MAX_LENGTH)
      {
            struct Point newTail = snake->body[snake->length - 1];

            if (snake->length < MAX_LENGTH)
            {
                  snake->body[snake->length] = newTail;

                  snake->length++;
            }
      }
}
void moveSnake(struct Snake *snake, uint8_t value);

void gameInput()
{
      switch (call_getChar())
      {
      case 'W':
      case 'w':
            moveSnake(&snakes[0], 1);
            break;
      case 'S':
      case 's':
            moveSnake(&snakes[0], 3);
            break;
      case 'D':
      case 'd':
            moveSnake(&snakes[0], 0);
            break;
      case 'A':
      case 'a':
            moveSnake(&snakes[0], 2);
            break;
      case 17:
            moveSnake(&snakes[1], 1);
            break;
      case 18:
            moveSnake(&snakes[1], 3);
            break;
      case 19:
            moveSnake(&snakes[1], 0);
            break;
      case 20:
            moveSnake(&snakes[1], 2);
            break;
      }
}