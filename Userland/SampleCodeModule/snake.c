#include <UserSyscalls.h>
#include <snake.h>
#include <menu.h>
#include <sound.h>

struct Snake snake;
uint32_t delayTicks = 1; // Modificarlo segun el grado de dificultad, cuanto mas alto mas dificil

uint32_t faceStartingX;
uint32_t faceStartingY;

void start_game()
{
      call_paintScreen(CARAMEL_BROWN);
      drawSnakeInterface(CARAMEL_BROWN);
      initializeSnake(&snake, 50, 120, SNAKE_COLOR);
      drawRandomFace();

      uint8_t flagWall = 0;
      uint8_t flagSnake = 0;

      uint32_t lastTick = call_ticks();
      uint32_t currentTick;

      uint32_t mapWidth = call_getWidth();
      uint32_t mapHeight = call_getHeight();

      while (flagWall == 0 && flagSnake == 0)
      {
            currentTick = call_ticks();
            gameInput();
            if (currentTick - lastTick >= delayTicks)
            {
                  call_drawRectangle(CARAMEL_BROWN, snake.body[snake.length - 1].x, snake.body[snake.length - 1].y, SQUARE_SIZE, SQUARE_SIZE);
                  updateSnake(&snake, mapWidth, mapHeight, &flagWall, &flagSnake, 0, faceStartingX, faceStartingY);
                  redrawSnake(&snake);

                  lastTick = currentTick;
            }
      }
      playEndingSound();

      drawSnakeEndingScreen(snake.length - INITIAL_LENGTH);
      while (1)
      {
            int letter = call_getChar();
            switch (letter)
            {
            case '\n':
                  start_game();
                  return;
            case 27:
                  return;
            default:
                  break;
            }
      }
      return;
}

int updateSnake(struct Snake *snake, uint32_t mapWidth, uint32_t mapHeight, uint8_t *flagWall, uint8_t *flagSnake, int gameMode, uint32_t faceX, uint32_t faceY)
{
      struct Point head;
      head.x = snake->body[0].x;
      head.y = snake->body[0].y;

      uint8_t eaten = 0;

      switch (snake->mov)
      {
      case DOWN:
            head.y += SQUARE_SIZE;
            eaten = checkSnakeEatRevamped(head.x, head.y, gameMode, faceX, faceY);
            break;
      case UP:
            head.y -= SQUARE_SIZE;
            eaten = checkSnakeEatRevamped(head.x, head.y, gameMode, faceX, faceY);
            break;
      case RIGHT:
            head.x += SQUARE_SIZE;
            eaten = checkSnakeEatRevamped(head.x, head.y, gameMode, faceX, faceY);
            break;
      case LEFT:
            head.x -= SQUARE_SIZE;
            eaten = checkSnakeEatRevamped(head.x, head.y, gameMode, faceX, faceY);
            break;
      }

      if (head.x >= mapWidth || head.x < 0 || head.y >= mapHeight || head.y < INTERFACE_LENGTH)
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
int checkSnakeEatRevamped(uint32_t headX, uint32_t headY, int gameMode, uint32_t faceX, uint32_t faceY)
{
      int eaten = 0;
      int headXCondition = (headX >= faceX - SQUARE_SIZE) && (headX <= faceX + SQUARE_SIZE);
      int headYCondition = (headY >= faceY - SQUARE_SIZE) && (headY <= faceY + SQUARE_SIZE);
      if (headXCondition && headYCondition)
      {
            if (gameMode == 1)
            { // Estamos en el juego para 2 jugadores
                  return 1;
            }
            call_beep(EAT_FREQ, 1);
            eat();
            call_drawRectangle(CARAMEL_BROWN, faceStartingX, faceStartingY, SQUARE_SIZE, SQUARE_SIZE);
            drawRandomFace();
            eaten = 1;
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
      if (eyeX < call_getWidth() && eyeY < call_getHeight())
      {
            call_drawCircle(RED, eyeX, eyeY, eyeRadius);
      }
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
            if (x < snake->body[i].x + SQUARE_SIZE && x + SQUARE_SIZE > snake->body[i].x &&
                y < snake->body[i].y + SQUARE_SIZE && y + SQUARE_SIZE > snake->body[i].y)
            {
                  return 1; // Colisiona
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

      uint32_t minX = SQUARE_SIZE;
      uint32_t minY = SQUARE_SIZE + INTERFACE_LENGTH;
      uint32_t maxX = call_getWidth() - SQUARE_SIZE;
      uint32_t maxY = call_getHeight() - SQUARE_SIZE;
      ;

      uint8_t collision = 1;
      do
      {
            faceStartingX = getRandom(minX, maxX);
            ;
            faceStartingY = getRandom(minY, maxY);

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

      setPoints(snake.length - INITIAL_LENGTH, CARAMEL_BROWN);
};

void gameInput()
{
      switch (call_getChar())
      {
      case 'W':
      case 'w':
            moveSnake(1, &snake);
            break;
      case 'S':
      case 's':
            moveSnake(0, &snake);
            break;
      case 'D':
      case 'd':
            moveSnake(2, &snake);
            break;
      case 'A':
      case 'a':
            moveSnake(3, &snake);
            break;
      }
}
