#pragma once

#include "ofMain.h"
#include "entity.h"

class Pulse : public Entity {
  public:
    static void setup();
    ////////////////////////////
    // Enums ///////////////////
    ////////////////////////////
    enum PulseType {
      DEFAULT,
      ORIGIN
    } pulseType;

    ////////////////////////////
    // STATIC METHODS //////////
    ////////////////////////////
    static void create(
      ofPoint position,
      double initialSize,
      PulseType pulseType
    );

    ////////////////////////////
    // STATIC DATA /////////////
    ////////////////////////////
    static vector<Pulse> collection;

    ////////////////////////////
    // CONSTRUCTOR /////////////
    ////////////////////////////
    Pulse(
      ofPoint position,
      double initialSize,
      PulseType pulseType
    );

    void death();

    ////////////////////////////
    // FRAME METHODS ///////////
    ////////////////////////////
    void update(double dt);
    void draw();

  protected:
    static deque<int> voices;
    void oscInit();
    void oscUpdate();

    double growthRate;
    double decayRate;
    double timeRemaining;
    double duration;
    ofFloatColor color;
    void grow(double dt);
};
