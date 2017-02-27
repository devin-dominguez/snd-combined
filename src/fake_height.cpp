#include "fake_height.h"
#include "projector.h"


FakeHeight::FakeHeight() {
  width = 160;
  height = 120;
  ratio = ofVec2f(1, 1);
  heightmap.allocate(width, height, OF_IMAGE_GRAYSCALE);
  for (size_t x = 0; x < width; x++) {
    for (size_t y = 0; y < height; y++) {
      char z = ofNoise((double)x * 0.025, (double)y * 0.025) * 128;
      heightmap.getPixels()[y * width + x] = z;
    }
  }
  heightmap.update();
}

void FakeHeight::setWorldDimensions(double worldWidth, double worldHeight) {
  ratio = ofVec2f((double)width / worldWidth, (double)height / worldHeight);
}


double FakeHeight::getHeightAt(ofPoint position) {
  position *= ratio;

  double w = width;
  double h = width;
  double x1 = (int)position.x;
  double x2 = x1 + 1;
  double y1 = (int)position.y;
  double y2 = y1 + 1;
  
  //clip to heightmap size to avoid potential disaster
  x1 = ofClamp(0, x1, w - 1);
  x2 = ofClamp(0, x2, w - 1);
  y1 = ofClamp(0, y1, h - 1);
  y2 = ofClamp(0, y2, h - 1);
  
  //lookup height values
  double z11 = heightmap.getPixels()[x1 + y1 * w];
  double z12 = heightmap.getPixels()[x1 + y2 * w];
  double z21 = heightmap.getPixels()[x2 + y1 * w];
  double z22 = heightmap.getPixels()[x2 + y2 * w];
  
  //interpolate
  double r1 = (x2 - position.x) * z11 + (position.x - x1) * z21;
  double r2 = (x2 - position.x) * z12 + (position.x - x1) * z22;
  double z = (y2 - position.y) * r1 + (position.y - y1) * r2;
  
  return z / 255.0;
}

void FakeHeight::draw(double width, double height) {
  ofPoint screenPosition = Projector::getCoords(ofPoint(0, 0));
  double screenWidth = Projector::getScale(width);
  double screenHeight = Projector::getScale(height);
  heightmap.draw(screenPosition.x, screenPosition.y, screenWidth, screenHeight);
}
