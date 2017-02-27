#pragma once
#include "ofMain.h"
#include "entity.h"
#include "depth_physics.h"

class Blimp : public Entity {
  public:
    Blimp(ofPoint position, double angle);
    static void create(ofPoint position, double anglge);

    void update(double dt);
    void draw();
    void collideWith(Entity* entity);

    static vector<Blimp> collection;

  private:
    DepthPhysics physics;

    ofPoint previousPosition;
    double initialSpeed;
    double minSize;
    double maxSize;
    double minSpeed;
    double allowedDownTime;
    double downTime;
    double angle;
    ofColor color;

    void move(double dt);
    void explode();
};

