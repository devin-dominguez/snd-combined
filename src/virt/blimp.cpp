#include "blimp.h"
#include "projector.h"
#include "explosion.h"
#include "trail.h"
#include "heightmap.h"
#include "depth_physics.h"
#include "gc_osc.h"

deque<int> Blimp::voices;
void Blimp::setup() {
  for (int i = 0; i < 16; i++) {
    voices.push_back(i);
  }
}

vector<Blimp> Blimp::collection;
void Blimp::create(ofPoint position, double angle) {
  collection.push_back(Blimp(position, angle));
}

Blimp::Blimp( ofPoint position, double angle) : Entity(position) {
  minSize = 10.0;
  maxSize = 20.0;
  size = minSize;
  minSpeed = 16.0;
  allowedDownTime = 0.15;

  z = HeightMap::map->getHeightAt(position);

  initialSpeed = 240.0;
  sqrSpeed = initialSpeed;

  fadeInTime = 0.125;
  fadeOutTime = 0.25;


  type = BLIMP;
  color = Entity::colors[2];
  previousPosition = position;

  PhysicsSettings settings;
  settings.mass = 16.0;
  settings.gravity = -981;
  settings.friction = 0.125;
  physics.setup(angle, initialSpeed, settings);
  move(1.0 / 60.0);
  oscInit();
}

void Blimp::death() {
  oscEvent("destroyed", 1);
  voices.push_back(voice);
}

void Blimp::oscInit() {
  voice = voices.front();
  voices.pop_front();
  oscEvent("created", 1);
  oscUpdate();
}

void Blimp::oscUpdate() {
  oscEvent("x", position.x);
  oscEvent("y", position.y);
  oscEvent("z", z);
  oscEvent("speed", sqrSpeed / 102400.0);
}

void Blimp::update(double dt) {
  switch(phase) {
    case NEW:
      fadeIn(dt);
      move(dt);
      break;
    case ALIVE:
      collidable = true;
      move(dt);
      break;
    case DYING:
      fadeOut(dt);
      collidable = false;;
      break;
    case DEAD:
      break;
  };

  oscUpdate();
}

void Blimp::move(double dt) {
  ofVec2f velocity = physics.getVelocityAtPoint(position, dt);
  position += velocity * dt;
  ofRectangle bounds(0, 0, 640, 480);
  if (!bounds.inside(position)) {
    explode();
    kill();
  }

  sqrSpeed = (velocity.x * velocity.x) + (velocity.y * velocity.y);
  if (sqrSpeed <= minSpeed) {
    downTime += dt;
  } else {
    downTime = 0;
  }

  if (downTime >= allowedDownTime) {
    //explode();
    kill();
  }

  z = HeightMap::map->getHeightAt(position);
  size = ofLerp(minSize, maxSize, z);

  BlimpTrail::create(position, previousPosition);
  previousPosition = position;
}

void Blimp::collideWith(Entity* entity) {
  switch(entity->type) {
    case NODE:
      explode();
      kill();
      break;
    default:
      break;
  }
}

void Blimp::explode() {
  for (size_t i = 0; i < 32; i++) {
    BlimpDebris::create(position);
  }
  vector<double> data;
  data.push_back(position.x);
  data.push_back(position.y);
  data.push_back(z);
  data.push_back(sqrSpeed / 102400.0);
  OSCHandler::specialEvent("blimp-explosion", data);
}

void Blimp::draw() {
  ofPoint screenPosition = Projector::getCoords(position);
  double screenSize = Projector::getScale(size);
  double angle = physics.getAngle();

 	ofPushMatrix();
	ofPushStyle();
		ofTranslate(screenPosition);
		ofRotate(angle * 180 / M_PI);
		ofSetColor(color, 255.0 * fadeLevel);
    ofFill();
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofDrawRectRounded(0, 0, screenSize * 2, screenSize * 2 * .5, 10);
		ofDrawTriangle(0, -.5 * screenSize, -screenSize, -screenSize, -.5 * screenSize, -.5 * screenSize);
		ofDrawTriangle(0, .5 * screenSize, -screenSize, screenSize, -.5 * screenSize, .5 * screenSize);
	ofPopStyle();
	ofPopMatrix();
}
