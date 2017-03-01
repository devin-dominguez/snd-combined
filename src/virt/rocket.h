#pragma once
#include "ofMain.h"
#include "entity.h"
#include "depth_physics.h"

class Rocket : public Entity {
  public:
    static void setup();
    Rocket(ofPoint position, double angle);
    static void create(ofPoint position, double anglge);

    void update(double dt);
    void draw();
    void death();
    void collideWith(Entity* entity);

    static vector<Rocket> collection;

  private:
    static deque<int> voices;
    void oscInit();
    void oscUpdate();

    DepthPhysics physics;
    double z;
    double sqrSpeed;

    double trailRate;
    double trailTime;

    double initialSpeed;
    double minSize;
    double maxSize;
    double minSpeed;
    double allowedDownTime;
    double downTime;
    double angle;
    ofColor color;

    double penetrationFadeOutTime;
    double penetrationAngle;

    bool penetrating;

    void move(double dt);
    void penetrate();
    void explode();
};
