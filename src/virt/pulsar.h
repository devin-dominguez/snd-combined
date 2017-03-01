#pragma once

#include "ofMain.h"
#include "entity.h"

class Pulsar : public Entity {
  public:
    static void setup();
    ////////////////////////////
    // STATIC METHODS //////////
    ////////////////////////////
    static void create(ofPoint position);

    ////////////////////////////
    // STATIC DATA /////////////
    ////////////////////////////
    static vector<Pulsar> collection;

    ////////////////////////////
    // CONSTRUCTOR /////////////
    ////////////////////////////
    Pulsar(ofPoint position);

    ////////////////////////////
    // FRAME METHODS ///////////
    ////////////////////////////
    void update(double dt);
    void draw();
    void death();

  private:
    static deque<int> voices;
    void oscInit();
    void oscUpdate();
    void action(double dt);
    void wobble(double dt);

    ofFloatColor color;

    double minInterval;
    double maxInterval;
    double decayRange;
    double minDecayRate;
    double decayRate;
    double minFrequency;
    double maxFrequency;

    double height;
    double timeElapsed;
    double phasor;

    ofVec2f freqScale;
    ofVec2f ampScale;

    double wobbleAmount;
    ofVec2f wobbleOffset;


};
