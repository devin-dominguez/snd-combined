#include "trail.h"
#include "projector.h"

Trail::Trail(ofPoint position) : Entity(position) {
  // nothing here!
}

void Trail::update(double dt) {
  switch(phase) {
    case NEW:
      fadeIn(dt);
      break;
    case ALIVE:
      duration -= dt;
      if (duration <= 0.0) {
        kill();
      }
      break;
    case DYING:
      fadeOut(dt);
      break;
    case DEAD:
      break;
  };
}

///////////////////////////////////////////////////////////////
// ROCKET TRAIL
///////////////////////////////////////////////////////////////

vector<RocketTrail> RocketTrail::collection;
void RocketTrail::create(ofPoint position) {
  collection.push_back(RocketTrail(position));
}

RocketTrail::RocketTrail(ofPoint position) : Trail(position) {
  color = Entity::colors[1];
  fadeInTime = 0.1;
  fadeOutTime = 2.0;
  duration = 0.1;
  size = 2.0;
}

void RocketTrail::draw() {
  ofPoint screenPosition = Projector::getCoords(position);
  double screenSize = Projector::getScale(size * fadeLevel);

 	ofPushMatrix();
	ofPushStyle();
		ofTranslate(screenPosition);
		ofSetColor(color, 255.0 * fadeLevel);
    ofNoFill();
    ofDrawCircle(0, 0, screenSize);
	ofPopStyle();
	ofPopMatrix();
}

///////////////////////////////////////////////////////////////
// BLIMP TRAIL
///////////////////////////////////////////////////////////////

vector<BlimpTrail> BlimpTrail::collection;
void BlimpTrail::create(ofPoint position, ofPoint previousPosition) {
  collection.push_back(BlimpTrail(position, previousPosition));
}

BlimpTrail::BlimpTrail(ofPoint position, ofPoint previousPosition) : Trail(position) {
  color = Entity::colors[1];
  fadeInTime = 0.1;
  fadeOutTime = 2.0;
  duration = 0.1;
  size = 2.0;
  this->previousPosition = previousPosition;
}

void BlimpTrail::draw() {
  ofPoint screenPosition = Projector::getCoords(position);
  ofPoint previousScreenPosition = Projector::getCoords(previousPosition);

	ofPushStyle();
		ofSetColor(color, 255.0 * fadeLevel);
    ofDrawLine(screenPosition, previousScreenPosition);
	ofPopStyle();
}
