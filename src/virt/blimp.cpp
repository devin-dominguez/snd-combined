#include "blimp.h"
#include "projector.h"
#include "explosion.h"
#include "trail.h"
#include "depth_physics.h"

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

  initialSpeed = 200.0;

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
}

void Blimp::move(double dt) {
  ofVec2f velocity = physics.getVelocityAtPoint(position, dt);
  position += velocity * dt;
  ofRectangle bounds(0, 0, 640, 480);
  if (!bounds.inside(position)) {
    explode();
    kill();
  }

  double sqrSpeed = (velocity.x * velocity.x) + (velocity.y * velocity.y);
  if (sqrSpeed <= minSpeed) {
    downTime += dt;
  } else {
    downTime = 0;
  }

  if (downTime >= allowedDownTime) {
    explode();
    kill();
  }

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
