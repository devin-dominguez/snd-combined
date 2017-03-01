#include "pulse.h"

#include "projector.h"
#include "gc_osc.h"

deque<int> Pulse::voices;
void Pulse::setup() {
  for (int i = 0; i < 8; i++) {
    voices.push_back(i);
  }
}

vector<Pulse> Pulse::collection;
void Pulse::create(
  ofPoint position,
  double initialSize,
  PulseType pulseType
) {
  collection.push_back(Pulse(position, initialSize, pulseType));
}

Pulse::Pulse(
  ofPoint position,
  double initialSize,
  PulseType pulseType = DEFAULT
) : Entity(position)
{

  size = initialSize;
  fadeOutTime = 1.0;
  duration = 5.0;
  type = PULSE;
  this->pulseType = pulseType;

  switch(pulseType) {
    case DEFAULT:
      color = Entity::colors[3];
      growthRate = 55.0;
      fadeInTime = 0.25;
      decayRate = 5.0;
      break;
    case ORIGIN:
      color = Entity::colors[1];
      growthRate = 120.0;
      fadeInTime = 0.1;
      decayRate = 20.0;
      break;
  }

  oscInit();
}

void Pulse::death() {
  oscEvent("destroyed", 1);
  voices.push_back(voice);
}

void Pulse::oscInit() {
  voice = voices.front();
  voices.pop_front();
  oscEvent("created", 1);
  oscEvent("type", pulseType);
  oscEvent("x", position.x);
  oscEvent("y", position.y);
  oscUpdate();
}

void Pulse::oscUpdate() {
  oscEvent("size", size);
}

void Pulse::update(double dt) {
  grow(dt);

  switch(phase) {
    case NEW:
      fadeIn(dt);
      break;
    case ALIVE:
      collidable = true;
      break;
    case DYING:
      fadeOut(dt);
      break;
    case DEAD:
      break;
  };

  oscUpdate();
}

void Pulse::grow(double dt) {
  if (duration > 0.0) {
    duration -= dt;
  } else {
    kill();
  }
  growthRate -= decayRate * dt;
  growthRate = fmax(0.0, growthRate);
  size += growthRate * dt;
}

void Pulse::draw() {
  ofPoint screenPosition = Projector::getCoords(position);
  double screenSize = Projector::getScale(size);

  ofPushStyle();
  ofPushMatrix();

  ofNoFill();
  ofSetColor(color, 255 * fadeLevel);
  ofTranslate(screenPosition);
  ofDrawCircle(0, 0, screenSize);

  ofPopStyle();
  ofPopMatrix();
}
