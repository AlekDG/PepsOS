#include <buildSnakeMap.h>
#include <stdint.h>

// faltaria ver como hacer para que el mapa ocupe toda la pantalla

void buildMap()
{
      int mapSize = 20;
      int squareSize = 40;
      uint32_t green = 0x15F03;
      uint32_t lightGreen = 0xC0E3AF;

      for (int i = 0; i < mapSize; i++)
      {
            for (int j = 0; j < mapSize; j++)
            {
                  int x = i * squareSize;
                  int y = j * squareSize;

                  if ((i + j) % 2 != 0)
                  {
                        drawRectangle(green, x, y, squareSize, squareSize);
                  }
                  else
                  {
                        drawRectangle(lightGreen, x, y, squareSize, squareSize);
                  }
            }
      }
}
