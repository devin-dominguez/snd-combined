#pragma once

#include "ofMain.h"
#include "entity.h"

class PulseNode {
  public:
    PulseNode();
    void setup(ofPoint nodePosition, double nodeSize);
    void action(double dt);
    void collideWith(Entity* entity);
    void draw(double fadeLevel);
    bool ready;

    double getNormalizedCoolDown();

  private:
    ofPoint nodePosition;
    double nodeSize;
    void cooling(double dt);
    void collisionAction();
    ofFloatColor color;
    double coolDownTime;
    double coolDown;

    bool colliding;
    bool collided;

    double maxSize;
    double size;
    double sizeTarget;
    double lerpRate;
    double rotationRate;
    double rotation;
    int spinnerResolution;
};
