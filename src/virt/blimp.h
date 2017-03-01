#pragma once
#include "ofMain.h"
#include "entity.h"
#include "depth_physics.h"

class Blimp : public Entity {
  public:
    static void setup();
    Blimp(ofPoint position, double angle);
    static void create(ofPoint position, double anglge);

    void update(double dt);
    void draw();
    void death();
    void collideWith(Entity* entity);

    static vector<Blimp> collection;

  private:
    static deque<int> voices;
    void oscInit();
    void oscUpdate();

    DepthPhysics physics;
    double z;

    ofPoint previousPosition;
    double initialSpeed;
    double minSize;
    double maxSize;
    double minSpeed;
    double sqrSpeed;
    double allowedDownTime;
    double downTime;
    double angle;
    ofColor color;

    void move(double dt);
    void explode();
};

