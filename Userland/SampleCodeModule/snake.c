#include <UserSyscalls.h>
#include <snake.h>

struct Snake snake;

// Habria que hacer un menu inicial para que cuando el jugador pierda pueda volver a jugar apretando ah

uint8_t delayTicks = 4; // Ajustar esto segun la dificultad(Crear una opcion en el menu inicial del juego en el que te pida seleccionar la dificultad).

uint32_t faceStartingX;
uint32_t faceStartingY;

void start_game()
{
      call_paintScreen(CARAMEL_BROWN);
      drawRandomFace();
      initializeSnake(&snake, 50, 50, WHITE);

      uint8_t flagWall = 0;
      uint8_t flagSnake = 0;

      uint32_t lastTick = call_ticks();
      uint32_t currentTick;
      unsigned int delay = delayTicks;

      uint32_t mapWidth = call_getWidth();
      uint32_t mapHeight = call_getHeight();

      while (flagWall == 0 && flagSnake == 0)
      {
            currentTick = call_ticks();

            if (currentTick - lastTick >= delay)
            {
                  call_drawRectangle(CARAMEL_BROWN, snake.body[snake.length - 1].x, snake.body[snake.length - 1].y, SQUARE_SIZE, SQUARE_SIZE);
                  updateSnake(&snake, mapWidth, mapHeight, &flagWall, &flagSnake, 0);
                  redrawSnake(&snake);

                  lastTick = currentTick;
            }
            // Agregar manejo de las teclas
      }
      // Hacer el display de lo q hizo juli
}

int updateSnake(struct Snake *snake, uint32_t mapWidth, uint32_t mapHeight, uint8_t *flagWall, uint8_t *flagSnake, int gameMode)
{
      struct Point head;
      head.x = snake->body[0].x;
      head.y = snake->body[0].y;

      uint8_t eaten = 0;

      switch (snake->mov)
      {
      case DOWN:
            head.y += SQUARE_SIZE;
            eaten = checkSnakeEatFace(SQUARE_SIZE, 0, head.x, head.y, gameMode);
            break;
      case UP:
            head.y -= SQUARE_SIZE;
            eaten = checkSnakeEatFace(SQUARE_SIZE, 0, head.x, head.y, gameMode);
            break;
      case RIGHT:
            head.x += SQUARE_SIZE;
            eaten = checkSnakeEatFace(SQUARE_SIZE, 0, head.x, head.y, gameMode);
            break;
      case LEFT:
            head.x -= SQUARE_SIZE;
            eaten = checkSnakeEatFace(SQUARE_SIZE, 0, head.x, head.y, gameMode);
            break;
      }

      if (head.x >= mapWidth || head.x < 0 || head.y >= mapHeight || head.y < 0)
      {
            *flagWall = 1;
            return eaten;
      }

      for (int i = 1; i < snake->length && !(*flagSnake); i++)
      {
            if (head.x == snake->body[i].x && head.y == snake->body[i].y)
            {
                  *flagSnake = 1;
                  return eaten;
            }
      }

      for (int i = snake->length - 1; i > 0; i--)
      {
            snake->body[i] = snake->body[i - 1];
      }

      snake->body[0] = head;
      return eaten;
}

void redrawSnake(struct Snake *snake)
{
      call_drawRectangle(CARAMEL_BROWN, snake->body[snake->length - 1].x, snake->body[snake->length - 1].y, SQUARE_SIZE, SQUARE_SIZE);

      for (int i = 1; i < snake->length; i++)
      {
            call_drawRectangle(snake->color, snake->body[i].x, snake->body[i].y, SQUARE_SIZE, SQUARE_SIZE);
      }

      drawSnakeHead(snake->body[0].x, snake->body[0].y, snake);
}

int checkSnakeEatFace(int offsetX, int offsetY, uint32_t headX, uint32_t headY, int gameMode)
{
      uint8_t eaten = 0;
      for (int i = 0; i <= offsetX && !eaten; i++)
      {
            for (int j = 0; j <= offsetY && !eaten; j++)
            {
                  if ((headX + i >= faceStartingX && headX + i <= faceStartingX + SQUARE_SIZE) &&
                      (headY + j >= faceStartingY && headY + j <= faceStartingY + SQUARE_SIZE))
                  {
                        if (gameMode == 1)
                        { // Estamos en el juego para 2 jugadores
                              return 1;
                        }
                        eat();
                        call_drawRectangle(CARAMEL_BROWN, faceStartingX, faceStartingY, SQUARE_SIZE, SQUARE_SIZE);
                        drawRandomFace();
                        eaten = 1;
                  }
            }
      }
      return eaten;
}

void moveSnake(uint8_t value, struct Snake *snake)
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

void drawSnakeHead(uint32_t x, uint32_t y, struct Snake *snake)
{
      call_drawRectangle(snake->color, x, y, SQUARE_SIZE, SQUARE_SIZE);

      uint32_t eyeX = x + (3 * SQUARE_SIZE / 4);
      uint32_t eyeY = y + (SQUARE_SIZE / 4);
      uint32_t eyeRadius = SQUARE_SIZE / 8;
      call_drawCircle(RED, eyeX, eyeY, eyeRadius);
}

void initializeSnake(struct Snake *snake, uint16_t startingX, uint16_t startingY, uint32_t snakeColor)
{
      snake->mov = DOWN;
      snake->length = 6;
      snake->color = snakeColor;

      for (int i = 0; i < snake->length; i++)
      {
            snake->body[i].x = startingX + (snake->length - 1 - i) * SQUARE_SIZE;
            snake->body[i].y = startingY;

            if (i == 0)
            {
                  drawSnakeHead(snake->body[i].x, snake->body[i].y, snake);
            }
            else
            {
                  call_drawRectangle(snake->color, snake->body[i].x, snake->body[i].y, SQUARE_SIZE, SQUARE_SIZE);
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

uint8_t checkSelfCollision(uint32_t x, uint32_t y, struct Snake *snake)
{
      for (int i = 0; i < snake->length; i++)
      {
            if (x <= snake->body[i].x + SQUARE_SIZE && x >= snake->body[i].x &&
                y <= snake->body[i].y + SQUARE_SIZE && y >= snake->body[i].y)
            {
                  return 1;
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

            collision = checkSelfCollision(faceStartingX, faceStartingY, &snake);
      } while (collision);

      call_drawFace(faceStartingX, faceStartingY, SQUARE_SIZE);
}

void eat()
{
      struct Point newTail;
      newTail.x = snake.body[snake.length - 1].x;
      newTail.y = snake.body[snake.length - 1].y;

      snake.body[snake.length] = newTail;
      snake.length++;
};

/*
void gameInput(){
    switch(call_getChar()){
        case 'W': case'w':
            moveSnake(1, &snake);
            break;
        case 'S': case's':
            moveSnake(3, &snake);
            break;
        case 'D': case 'd':
            moveSnake(0, &snake);
            break;
        case 'A': case 'a':
            moveSnake(2, &snake);
            break;
        /*TODO: Decomment when multiplayer is implemented
        case 17:
            moveP2(1, &snake);
            break;
        case 18:
            moveP2(3, &snake);
            break;
        case 19:
            moveP2(0, &snake);
            break;
        case 20:
            moveP2(2, &snake);
            break;
    }

}
*/