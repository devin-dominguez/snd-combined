#pragma once

#include "ofMain.h"
#include "entity.h"

class Trail : public Entity {
  public:
    Trail(ofPoint position);
    void update(double dt);

  protected:
    ofColor color;
    double duration;
};

class RocketTrail : public Trail {
  public:
    RocketTrail(ofPoint position);
    static void create(ofPoint position);
    static vector<RocketTrail> collection;
    void draw();

  private:
    double size;
    double initialTime;
};

class BlimpTrail : public Trail {
  public:
    BlimpTrail(ofPoint position, ofPoint previousPosition);
    static void create(ofPoint position, ofPoint previousPosition);
    static vector<BlimpTrail> collection;
    void draw();

  private:
    ofPoint previousPosition;
    double size;
    double initialTime;
};
