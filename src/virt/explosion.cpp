#include "explosion.h"
#include "projector.h"

Explosion::Explosion(ofPoint position) : Entity(position) {
  fadeInTime = 0.1;
  fadeOutTime = 1.25;
}

void Explosion::update(double dt) {
  move(dt);
  switch(phase) {
    case NEW:
      fadeIn(dt);
      break;
    case ALIVE:
      break;
    case DYING:
      fadeOut(dt);
      break;
    case DEAD:
      break;
  };
}

void Explosion::move(double dt) {
  position += velocity * dt;
  velocity *= decayFactor;
  duration -= dt;
  if (duration <= 0.0) {
    kill();
  }
}

void Explosion::draw() {
  ofPoint screenPosition = Projector::getCoords(position);
  double screenSize = Projector::getScale(size);

 	ofPushMatrix();
	ofPushStyle();
		ofTranslate(screenPosition);
		ofSetColor(color, 255.0 * fadeLevel);
    ofFill();
    ofDrawCircle(0, 0, screenSize);
	ofPopStyle();
	ofPopMatrix();
}

vector<Blood> Blood::collection;
void Blood::create(ofPoint position, double angle) {
  collection.push_back(Blood(position, angle));
}

Blood::Blood(ofPoint position, double angle) : Explosion(position) {
  color = Entity::colors[3];
  duration = 2.0;
  decayFactor = ofRandom(0.85, 0.99);
  size = ofRandom(0.5, 2.5);
  angle += ofRandom(-M_PI / 6.0, M_PI / 6.0);
  velocity = ofVec2f(cos(angle), sin(angle));
  velocity *= ofRandom(24.0, 96.0);
}

vector<Debris> Debris::collection;
void Debris::create(ofPoint position) {
  collection.push_back(Debris(position));
}

Debris::Debris(ofPoint position) : Explosion(position) {
  color = Entity::colors[1];
  duration = 2.5;
  decayFactor = ofRandom(0.9, 0.98);
  size = ofRandom(0.5, 1.5);
  double angle = ofRandom(0.0, M_PI * 2.0);
  velocity = ofVec2f(cos(angle), sin(angle));
  velocity *= ofRandom(24.0, 96.0);
}

vector<BlimpDebris> BlimpDebris::collection;
void BlimpDebris::create(ofPoint position) {
  collection.push_back(BlimpDebris(position));
}

BlimpDebris::BlimpDebris(ofPoint position) : Explosion(position) {
  color = Entity::colors[1];
  duration = 0.5;
  fadeOutTime = 5.0;
  decayFactor = ofRandom(0.95, 0.99);
  size = ofRandom(2.5, 4.5);
  double angle = ofRandom(0.0, M_PI * 2.0);
  velocity = ofVec2f(cos(angle), sin(angle));
  velocity *= ofRandom(96.0, 120.0);
}

