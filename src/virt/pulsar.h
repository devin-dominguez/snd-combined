#pragma once

#include "ofMain.h"
#include "entity.h"

class Pulsar : public Entity {
  public:
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

  private:
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

    double timeElapsed;
    double phasor;

    ofVec2f freqScale;
    ofVec2f ampScale;

    double wobbleFreq;
    double wobbleAmp;
    ofVec2f wobbleOffset;


};
