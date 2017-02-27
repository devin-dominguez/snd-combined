#pragma once

#include "ofMain.h"

class HeightMap {
  public:
    virtual double getHeightAt(ofPoint position) = 0;
    virtual void setWorldDimensions(double worldWidth, double worldHeight) = 0;
    static void setHeightMap(HeightMap* map);
    static HeightMap* map;
};
