#pragma once

#include "ofMain.h"
#include "heightmap.h"

class FakeHeight : public HeightMap {
  public:
    FakeHeight();
    double getHeightAt(ofPoint position);
    void setWorldDimensions(double worldWidth, double worldHeight);
    void draw(double width, double height);

  private:
     ofImage heightmap;
     int width;
     int height;
     double worldWidth;
     double worldHeight;
     ofVec2f ratio;
};
