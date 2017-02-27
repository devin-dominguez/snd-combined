#include "heightmap.h"

HeightMap* HeightMap::map;

void HeightMap::setHeightMap(HeightMap* map) {
  HeightMap::map = map;
}
