#pragma once

#include "ofMain.h"
#include "entity.h"

class Explosion : public Entity {
  public:
    Explosion(ofPoint position);
    void update(double dt);
    void draw();

  protected:
    ofColor color;
    ofVec2f velocity;
    double decayFactor;
    double duration;

    void move(double dt);
};

class Blood : public Explosion {
  public:
    Blood(ofPoint position, double angle);
    static void create(ofPoint position, double angle);
    static vector<Blood> collection;
};

class Debris : public Explosion {
  public:
    Debris(ofPoint position);
    static void create(ofPoint position);
    static vector<Debris> collection;
};

class BlimpDebris : public Explosion {
  public:
    BlimpDebris(ofPoint position);
    static void create(ofPoint position);
    static vector<BlimpDebris> collection;
};
