#pragma once
#include "ofMain.h"
#include "entity.h"
#include "depth_physics.h"

class Rocket : public Entity {
  public:
    Rocket(ofPoint position, double angle);
    static void create(ofPoint position, double anglge);

    void update(double dt);
    void draw();
    void collideWith(Entity* entity);

    static vector<Rocket> collection;

  private:
    DepthPhysics physics;

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
