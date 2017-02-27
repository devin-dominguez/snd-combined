#pragma once

#include "ofMain.h"

struct PhysicsSettings {
  double mass;
  double gravity;
  double friction;
};

class DepthPhysics {
  public:
    void setup(double angle, double speed, PhysicsSettings settings);
    ofVec2f getVelocityAtPoint(ofPoint position, double dt);
    double getAngle();

  private:
    double getIncline(double angle, double distance);
    ofVec2f getCurveForces();
    double mass;
    double gravity;
    double friction;

    ofVec2f velocity;
    ofPoint position;

};
