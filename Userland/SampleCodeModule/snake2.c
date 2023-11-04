#include <UserSyscalls.h>
#include <snake.h>
#include <snake2.h>

struct Snake snakeP1;
struct Snake snakeP2;

uint32_t faceStartingX2;
uint32_t faceStartingY2;

uint8_t delayTicksTwo = 3; // Ajustar esto segun la dificultad(Crear una opcion en el menu inicial del juego en el que te pida seleccionar la dificultad).

// Habria que hacer un menu inicial para que cuando el jugador pierda pueda volver a jugar apretando ah

void start_gameTwo()
{
    call_paintScreen(CARAMEL_BROWN);
    initializeSnake(&snakeP1, 50, 50, PURPLE);
    initializeSnake(&snakeP2, 300, 80, WHITE);
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

    if (eaten1 || !flagSnakeP1 || !flagWallP1)
    {
        // gano P1
    }
    else
    {
        // gano P2
    }
}

void eatTwo(struct Snake *snake){
    // Mostrar menu que muestre que termino el juego
};

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

    uint32_t minX = FACE_RADIUS;
    uint32_t minY = FACE_RADIUS;
    uint32_t maxX = call_getWidth() - FACE_RADIUS;
    uint32_t maxY = call_getHeight() - FACE_RADIUS;

    uint8_t collision = 1;
    uint8_t collision2 = 1;

    faceStartingX2 = getRandomTwo(minX, maxX - FACE_RADIUS);
    faceStartingY2 = getRandomTwo(minY, maxY - FACE_RADIUS);

    call_drawFace(faceStartingX2, faceStartingY2, SQUARE_SIZE);
}

void gameInput()
{
    switch (call_getChar())
    {
    case 'W':
    case 'w':
        moveSnake(1, &snakeP1);
        break;
    case 'S':
    case 's':
        moveSnake(3, &snakeP1);
        break;
    case 'D':
    case 'd':
        moveSnake(0, &snakeP1);
        break;
    case 'A':
    case 'a':
        moveSnake(2, &snakeP1);
        break;
    case 17:
        moveP2(1, &snakeP2);
        break;
    case 18:
        moveP2(3, &snakeP2);
        break;
    case 19:
        moveP2(0, &snakeP2);
        break;
    case 20:
        moveP2(2, &snakeP2);
        break;
    }
}
