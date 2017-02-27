#pragma once

#include "ofMain.h"
#include "entity.h"
#include "pulse_node.h"
#include "rocket_node.h"
#include "blimp_node.h"

class Node : public Entity {
  public:
    ////////////////////////////
    // STATIC METHODS //////////
    ////////////////////////////
    static void create(ofPoint position);

    ////////////////////////////
    // STATIC DATA /////////////
    ////////////////////////////
    static vector<Node> collection;

    ////////////////////////////
    // CONSTRUCTOR /////////////
    ////////////////////////////
    Node(ofPoint position);

    ////////////////////////////
    // FRAME METHODS ///////////
    ////////////////////////////
    void update(double dt);
    void draw();

    void collideWith(Entity* entity);

    bool pulseReady();

  protected:
    PulseNode pulseNode;
    RocketNode rocketNode;
    BlimpNode blimpNode;
    double pulsateAmpMin;
    double pulsateAmpMax;
    double pulsateFreqMin;
    double pulsateFreqMax;
    double pulsate;
    double phasor;

    void action(double dt);

    ofFloatColor color;

    double blimpCoolDown;
    double blimpCoolDownTime;

    double pulseCoolDown;
    double pulseCoolDownTime;

    bool originPulseHit;
    bool originPulseHitting;

    ////////////////////////////
    // ANIMATION STUFF /////////
    ////////////////////////////
    ofVec2f displacement;
    ofVec2f wobble;
    double snapOutRate;
    double snapInRate;

};
