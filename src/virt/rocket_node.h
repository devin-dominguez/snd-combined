#pragma once

#include "ofMain.h"
#include "entity.h"

class RocketNode {
  public:
    RocketNode();
    void setup(ofPoint nodePosition, double nodeSize);
    void action(double dt);
    void collideWith(Entity* entity);
    void draw(double fadeLevel);

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
    bool ready;

    int numRockets;

    double maxSize;
    double size;
    double sizeTarget;
    double lerpRate;
    double rotationRate;
    double rotation;
};

