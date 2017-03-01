#include "pulsar.h"
#include "projector.h"
#include "pulse.h"
#include "heightmap.h"

deque<int> Pulsar::voices;
void Pulsar::setup() {
  for (int i = 0; i < 4; i++) {
    voices.push_back(i);
  }
}

vector<Pulsar> Pulsar::collection;
void Pulsar::create(ofPoint position) {
  collection.push_back(Pulsar(position));
}

Pulsar::Pulsar(ofPoint position) : Entity(position) {
  size = 18.0;
  color = Entity::colors[1];
  type = PULSAR;

  phasor = 0.0;
  minFrequency = 1.0 / 18.0;
  maxFrequency = 1.0 / 6.0;

  decayRange = 0.25;
  minDecayRate = 0.25;
  decayRate = minDecayRate;

  freqScale = ofVec2f(27, 31);
  ampScale = ofVec2f(4, 6);
  wobbleAmount = 0.0;
  wobbleOffset = ofVec2f(0.0, 0.0);

  height = 0.0;
  oscInit();
}

void Pulsar::death() {
  oscEvent("destroyed", 1);
  voices.push_back(voice);
}

void Pulsar::oscInit() {
  voice = voices.front();
  voices.pop_front();
  oscEvent("created", 1);
  oscEvent("x", position.x);
  oscEvent("y", position.y);
}

void Pulsar::oscUpdate() {
  oscEvent("height", height);
  oscEvent("wobble", wobbleAmount);
}

void Pulsar::update(double dt) {
  switch(phase) {
    case NEW:
      fadeIn(dt);
      break;
    case ALIVE:
      action(dt);
      break;
    case DYING:
      fadeOut(dt);
      break;
    case DEAD:
      break;
  };

  oscUpdate();
}

void Pulsar::action(double dt) {
  // get average depth at location
  height = 0.0;
  for (size_t x = 0; x < size; x++) {
    for (size_t y = 0; y < size; y++) {
      ofPoint offset(x - size * 0.5, y - size * 0.5);
      height += HeightMap::map->getHeightAt(offset + position);
    }
  }
  height /= size * size;

  double frequency = ofLerp(minFrequency, maxFrequency, height);

  if (phasor < 1.0) {
    phasor += dt * frequency;
  } else {
    decayRate = minDecayRate;
    phasor -= 1.0;
    Pulse::create(position, 20, Pulse::ORIGIN);
    oscEvent("trigger", 1);
  }

  wobble(dt);
}

void Pulsar::wobble(double dt) {
  double nPhase = 0.0;
  if (phasor > decayRange) {
    nPhase = (phasor - decayRange) / (1.0 - decayRange);
    nPhase *= nPhase;
  }
  wobbleAmount = fmax(nPhase, wobbleAmount - decayRate * dt);
  decayRate += 0.75 * dt;

	wobbleOffset.x = cos(phasor * M_PI * 2 * freqScale.x * wobbleAmount);
	wobbleOffset.y = sin(phasor * M_PI * 2 * freqScale.y * wobbleAmount);
  wobbleOffset *= wobbleAmount * ampScale;
}

void Pulsar::draw() {
  ofPoint screenPosition = Projector::getCoords(position + wobbleOffset);
  double screenSize = Projector::getScale(size);
  double pulseScreenSize = Projector::getScale(size * 0.75 * phasor * phasor);

  ofPushMatrix();
    ofTranslate(screenPosition);
    ofPushStyle();
      ofEnableAlphaBlending();
      ofFill();
      // base
      ofSetColor(color, 255.0 * fadeLevel);
      ofDrawCircle(0, 0, screenSize);
      // masking
      ofSetColor(Entity::colors[0], 255.0 * fadeLevel);
      ofDrawCircle(0, 0, 0.75 * screenSize);
      // inner wave
      ofSetColor(color, 255.0 * fadeLevel * phasor );
      ofDrawCircle(0, 0, pulseScreenSize);
      // inner masking
      ofSetColor(Entity::colors[0], 255.0 * fadeLevel);
      ofDrawCircle(0, 0, pulseScreenSize - 2.0);
    ofPopStyle();
  ofPopMatrix();
}
