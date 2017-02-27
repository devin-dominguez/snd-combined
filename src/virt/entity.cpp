#include "entity.h"

////////////////////////////
// CONSTRUCTOR /////////////
////////////////////////////
Entity::Entity(ofPoint position) {
  this->position = position;
  size = 0.0;
  collidable = false;
  colliding = false;
  fadeInTime = 0.0;
  fadeOutTime = 0.0;
  phase = NEW;
  fadeLevel = 0.0;
  type = DEFAULT;
}

////////////////////////////
// GETTERS /////////////////
////////////////////////////
ofPoint Entity::getPosition() { return position; }
double Entity::getSize() { return size; }
bool Entity::isDead() { return phase == DEAD; }
bool Entity::isCollidable() { return collidable; }

////////////////////////////
// METHODS /////////////////
////////////////////////////
void Entity::fadeIn(double dt) {
  if (fadeInTime <= 0.0 ) {
    fadeInTime = 0.00000001;
  }

  fadeLevel += (1.0 / ofGetFrameRate()) / fadeInTime;
  if (fadeLevel > 1.0) {
    fadeLevel = 1.0;
    phase = ALIVE;
  }
}

void Entity::kill() {
  phase = DYING;
}

void Entity::fadeOut(double dt) {
  if (fadeOutTime <= 0.0 ) {
    fadeOutTime = 0.00000001;
  }

  fadeLevel -= (1.0 / ofGetFrameRate()) / fadeOutTime;
  if (fadeLevel < 0.0) {
    fadeLevel = 0.0;
    phase = DEAD;
  }
}

void Entity::collideWith(Entity* entity) {
  colliding = true;
}

////////////////////////////
// STATIC METHODS //////////
////////////////////////////
bool Entity::areColliding(Entity &entityA, Entity &entityB) {
  if ( !(entityA.isCollidable() && entityB.isCollidable()) ) {
    return false;
  }
  double a = entityA.getSize() - entityB.getSize();
  a *= a;

  double b1 = entityA.getPosition().x - entityB.getPosition().x;
  b1 *= b1;

  double b2 = entityA.getPosition().y - entityB.getPosition().y;
  b2 *= b2;

  double b = b1 + b2;

  double c = entityA.getSize() + entityB.getSize();
  c *= c;

  bool yes = a <= b && b <= c;
  return a <= b && b <= c;
}

////////////////////////////
// COLOR TABLE /////////////
////////////////////////////
ofFloatColor Entity::colors[] = {
  ofFloatColor(0.0, 0.0, 0.0),
  ofFloatColor(1.0, 1.0, 1.0),
  ofFloatColor(1.0, 0.0, 0.0),
  ofFloatColor(0.0, 1.0, 0.0),
  ofFloatColor(0.0, 0.0, 1.0)

};
