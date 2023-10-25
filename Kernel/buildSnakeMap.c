#include <buildSnakeMap.h>
#include <stdint.h>
#include "video.h"

// faltaria ver como hacer para que el mapa ocupe toda la pantalla

void buildMap()
{
      int squareSize = 40;
      uint32_t caramelBrown = 0x613613;

      drawRectangle(caramelBrown, 0, 0, getFullWidth(), getFullHeight());
}
