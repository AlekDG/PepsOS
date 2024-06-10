#include <UserSyscalls.h>
#include <snake.h>
#include <snake2.h>
#include <menu.h>
#include <sound.h>

#define DEFAULT_DIFFICULTY_LEVEL 1

#define PLAYER_TWO_STARTING_X 300
#define PLAYER_TWO_STARTING_Y 120


struct Snake snakeP1;
struct Snake snakeP2;

uint32_t faceStartingX2;
uint32_t faceStartingY2;

uint8_t delayTicksTwo = DEFAULT_DIFFICULTY_LEVEL; // Modificarlo segun el grado de dificultad, cuanto mas alto mas dificil

void start_gameTwo()
{
    call_paintScreen(CARAMEL_BROWN);
    draw2pSnake(CARAMEL_BROWN);
    initializeSnake(&snakeP1, PLAYER_ONE_STARTING_X, PLAYER_ONE_STARTING_Y, WHITE);
    initializeSnake(&snakeP2, PLAYER_TWO_STARTING_X, PLAYER_TWO_STARTING_Y, PURPLE);
    drawRandomFaceTwo();

    uint8_t flagWallP1 = 0;
    uint8_t flagSnakeP1 = 0;
    uint8_t flagWallP2 = 0;
    uint8_t flagSnakeP2 = 0;

    uint32_t lastTick = call_ticks();
    uint32_t currentTick;
    unsigned int delay = delayTicksTwo;

    uint32_t mapWidth = call_getWidth();
    uint32_t mapHeight = call_getHeight();

    uint8_t eaten1 = 0;
    uint8_t eaten2 = 0;

    while (!flagWallP1 && !flagSnakeP1 && !flagWallP2 && !flagSnakeP2 && !eaten1 && !eaten2)
    {
        currentTick = call_ticks();
        gameInputTwo();
        if (currentTick - lastTick >= delay)
        {
            call_drawRectangle(CARAMEL_BROWN, snakeP1.body[snakeP1.length - 1].x, snakeP1.body[snakeP1.length - 1].y, SQUARE_SIZE, SQUARE_SIZE);
            call_drawRectangle(CARAMEL_BROWN, snakeP2.body[snakeP2.length - 1].x, snakeP2.body[snakeP2.length - 1].y, SQUARE_SIZE, SQUARE_SIZE);
            eaten1 = updateSnake(&snakeP1, mapWidth, mapHeight, &flagWallP1, &flagSnakeP1, 1, faceStartingX2, faceStartingY2);
            redrawSnake(&snakeP1);
            if (!eaten1)
            {
                eaten2 = updateSnake(&snakeP2, mapWidth, mapHeight, &flagWallP2, &flagSnakeP2, 1, faceStartingX2, faceStartingY2);
                redrawSnake(&snakeP2);
            }

            lastTick = currentTick;
        }
    }
    playMegalovania();

    if (eaten1 || (!flagSnakeP1 && !flagWallP1 && !eaten2))
    {
        draw2pEnding(1);
        while (1)
        {
            int letter = call_getChar();
            switch (letter)
            {
            case '\n':
                start_gameTwo();
                return;
            case ESCAPE:
                return;
            default:
                break;
            }
        }
        return;
    }
    else
    {
        draw2pEnding(2);
        while (1)
        {
            int letter = call_getChar();
            switch (letter)
            {
            case '\n':
                start_gameTwo();
                return;
            case ESCAPE:
                call_exit();
            default:
                break;
            }
        }
        call_exit();
    }
}

uint32_t seedTwo;

uint32_t randTwo_()
{
    seedTwo = (seedTwo * 1664525 + 1013904223) & 0xFFFFFFFF; // Linear Congruential Generator
    return seedTwo;
}

uint32_t getRandomTwo(uint32_t min, uint32_t max)
{
    return (randTwo_() % (max - min + 1)) + min;
}

void drawRandomFaceTwo()
{
    static uint8_t initialized = 0;

    if (!initialized)
    {
        seedTwo = call_seconds();
        initialized = 1;
    }

    uint32_t minX = SQUARE_SIZE;
    uint32_t minY = SQUARE_SIZE + INTERFACE_LENGTH;
    ;
    uint32_t maxX = call_getWidth() - SQUARE_SIZE;
    uint32_t maxY = call_getHeight() - SQUARE_SIZE;

    uint8_t collisionSnakeP1 = 1;
    uint8_t collisionSnakeP2 = 1;
    do
    {
        faceStartingX2 = getRandomTwo(minX, maxX);
        faceStartingY2 = getRandomTwo(minY, maxY);

        collisionSnakeP1 = checkSelfCollision(faceStartingX2, faceStartingY2, &snakeP1);
        collisionSnakeP2 = checkSelfCollision(faceStartingX2, faceStartingY2, &snakeP2);
    } while (collisionSnakeP1 || collisionSnakeP2);

    call_drawFace(faceStartingX2, faceStartingY2, SQUARE_SIZE);
}

uint8_t checkSelfCollisionTwo(uint32_t x, uint32_t y, struct Snake *snake)
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

void gameInputTwo()
{
    switch (call_getChar())
    {
    case 'W':
    case 'w':
        moveSnake(1, &snakeP1);
        break;
    case 'S':
    case 's':
        moveSnake(0, &snakeP1);
        break;
    case 'D':
    case 'd':
        moveSnake(2, &snakeP1);
        break;
    case 'A':
    case 'a':
        moveSnake(3, &snakeP1);
        break;
    case 17:
        moveSnake(1, &snakeP2);
        break;
    case 18:
        moveSnake(3, &snakeP2);
        break;
    case 19:
        moveSnake(2, &snakeP2);
        break;
    case 20:
        moveSnake(0, &snakeP2);
        break;
    }
}
