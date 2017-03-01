#include "node.h"
#include "projector.h"
#include "pulse.h"

deque<int> Node::voices;
void Node::setup() {
  for (int i = 0; i < 8; i++) {
    voices.push_back(i);
  }
}

vector<Node> Node::collection;
void Node::create(ofPoint position) {
  collection.push_back(Node(position));
}

Node::Node(ofPoint position) : Entity(position) {
  size = 24.0;
  fadeInTime = 1.0;
  fadeOutTime = 1.0;
  type = NODE;
  color = Entity::colors[4];

  pulsate = 0.0;
  pulsateAmpMin = 0.1;
  pulsateAmpMax = 0.3;
  pulsateAmp = pulsateAmpMin;
  pulsateFreqMin = 0.125;
  pulsateFreqMax = 0.2;
  pulsateFreq = pulsateFreqMin;
  phasor = 0.0;

  pulseNode.setup(position, size);
  rocketNode.setup(position, size);
  blimpNode.setup(position, size);

  oscInit();
}

void Node::death() {
  oscEvent("destroyed", 1);
  voices.push_back(voice);
}

void Node::oscInit() {
  voice = voices.front();
  voices.pop_front();
  oscEvent("created", 1);
  oscEvent("x", position.x);
  oscEvent("y", position.y);
}

void Node::oscUpdate() {
  oscEvent("pulsate_freq", pulsateFreq);
  oscEvent("pulsate_amp", pulsateAmp);
}

void Node::update(double dt) {
  switch(phase) {
    case NEW:
      fadeIn(dt);
      break;
    case ALIVE:
      collidable = true;
      action(dt);
      break;
    case DYING:
      fadeOut(dt);
      collidable = false;
      break;
    case DEAD:
      break;
  };

  oscUpdate();
}

bool Node::pulseReady() {
  return pulseNode.ready;
}

void Node::collideWith(Entity* entity) {
  switch(entity->type) {
    case PULSE:
      switch( ((Pulse*)entity)->pulseType) {
        case Pulse::ORIGIN:
          rocketNode.collideWith(entity);
          break;
        case Pulse::DEFAULT:
          blimpNode.collideWith(entity);
          break;
      }
      break;
    case ROCKET:
      pulseNode.collideWith(entity);
      break;
    default:
      break;
  }
}

void Node::action(double dt) {
  pulseNode.action(dt);
  rocketNode.action(dt);
  blimpNode.action(dt);

  double coolDown = pulseNode.getNormalizedCoolDown();
  pulsateAmp = ofLerp(pulsateAmpMin, pulsateAmpMax, coolDown);
  pulsateFreq = ofLerp(pulsateFreqMin, pulsateFreqMax, coolDown);
  phasor += dt;
  pulsate = cos(M_PI * 2.0 * pulsateFreq * phasor);
  pulsate = (pulsate + 1.0) / 2.0;
  pulsate = pulsate * pulsate;
  pulsate = 1.0 - pulsate;
  pulsate *= -pulsateAmp;
  pulsate += 1.0;
}

void Node::draw() {
  ofPoint screenPosition = Projector::getCoords(position);
  double screenSize = Projector::getScale(size);

  ofPushMatrix();
    ofTranslate(screenPosition);
    rocketNode.draw(fadeLevel);
    pulseNode.draw(fadeLevel);
    blimpNode.draw(fadeLevel);

    // core
    ofPushStyle();
      ofSetColor(color, 255.0 * fadeLevel * 0.75);
      ofDrawCircle(0, 0, screenSize);
      ofSetColor(Entity::colors[0], 255.0 * fadeLevel);
      ofDrawCircle(0, 0, 0.75 * screenSize * pulsate);
      ofSetColor(255);
    ofPopStyle();

  ofPopMatrix();
}
