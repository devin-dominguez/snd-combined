#include "rocket.h"
#include "node.h"
#include "projector.h"
#include "depth_physics.h"
#include "heightmap.h"
#include "explosion.h"
#include "trail.h"
#include "gc_osc.h"

deque<int> Rocket::voices;
void Rocket::setup() {
  for (int i = 0; i < 64; i++) {
    voices.push_back(i);
  }
}

vector<Rocket> Rocket::collection;
void Rocket::create(ofPoint position, double angle) {
  collection.push_back(Rocket(position, angle));
}

Rocket::Rocket( ofPoint position, double angle) : Entity(position) {
  minSize = 10.0;
  maxSize = 20.0;
  size = minSize;
  minSpeed = 12.0;
  allowedDownTime = 0.15;

  z = HeightMap::map->getHeightAt(position);
  penetrationAngle = 0.0;

  initialSpeed = 280.0;
  sqrSpeed = initialSpeed;

  fadeInTime = 0.125;
  fadeOutTime = 0.25;

  trailRate = 0.125;
  trailTime = trailRate;

  type = ROCKET;
  color = Entity::colors[2];

  penetrating = false;
  penetrationFadeOutTime = 0.1;

  PhysicsSettings settings;
  settings.mass = 12.0;
  settings.gravity = -981;
  settings.friction = 0.1;
  physics.setup(angle, initialSpeed, settings);

  move(1.0 / 60.0);
  oscInit();
}

void Rocket::death() {
  oscEvent("destroyed", 1);
  voices.push_back(voice);
}

void Rocket::oscInit() {
  voice = voices.front();
  voices.pop_front();
  oscEvent("created", 1);
  oscUpdate();
}

void Rocket::oscUpdate() {
  oscEvent("x", position.x);
  oscEvent("y", position.y);
  oscEvent("z", z);
  oscEvent("speed", sqrSpeed / 102400.0);
  oscEvent("penetrating", penetrating);
}

void Rocket::update(double dt) {
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
      if (penetrating) {
        move(dt);
        penetrate();
      }
      collidable = false;;
      break;
    case DEAD:
      break;
  };

  oscUpdate();
}

void Rocket::move(double dt) {
  ofVec2f velocity = physics.getVelocityAtPoint(position, dt);
  position += velocity * dt;
  ofRectangle bounds(0, 0, 640, 480);
  if (!bounds.inside(position)) {
    explode();
    kill();
  }

  trailTime -= dt;
  if (trailTime <= 0.0) {
    trailTime = trailRate;
    RocketTrail::create(position);
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
}

void Rocket::collideWith(Entity* entity) {
  switch(entity->type) {
    case NODE:
      if (((Node*)entity)->pulseReady()) {
        penetrating = true;
        fadeOutTime = 0.1;
        ofVec2f deltaPosition = position - entity->getPosition();
        penetrationAngle = atan2(deltaPosition.y, deltaPosition.x);
      } else {
        explode();
      }
      kill();
      break;
    default:
      break;
  }
}

void Rocket::explode() {
  for (size_t i = 0; i < 16; i++) {
    Debris::create(position);
  }
  vector<double> data;
  data.push_back(position.x);
  data.push_back(position.y);
  data.push_back(z);
  data.push_back(sqrSpeed / 102400.0);
  OSCHandler::specialEvent("rocket-explosion", data);
}

void Rocket::penetrate() {
  for (size_t i = 0; i < 16; i++) {
    Blood::create(position, penetrationAngle);
  }
}

void Rocket::draw() {
  ofPoint screenPosition = Projector::getCoords(position);
  double screenSize = Projector::getScale(size);
  double angle = physics.getAngle();

 	ofPushMatrix();
	ofPushStyle();
		ofTranslate(screenPosition);
		ofRotate(angle * 180 / M_PI);
		ofSetColor(color, 255.0 * fadeLevel);
    ofFill();
		ofBeginShape();
			ofVertex(screenSize, 0);
			ofVertex(-screenSize, screenSize / 2.0);
			ofVertex(0, 0);
			ofVertex(-screenSize, -screenSize / 2.0);
			ofVertex(screenSize, 0);
		ofEndShape();
	ofPopStyle();
	ofPopMatrix();
}
