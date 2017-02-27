#include "depth_physics.h"
#include "heightmap.h"

void DepthPhysics::setup(double angle, double speed, PhysicsSettings settings) {
  mass = settings.mass;
  gravity = settings.gravity;
  friction = settings.friction;

  velocity = ofVec2f(cos(angle), sin(angle));
  velocity *= speed;
  position = ofPoint(0, 0);
}

double DepthPhysics::getAngle() {
  return atan2(velocity.y, velocity.x);
}


double DepthPhysics::getIncline(double angle, double distance) {
	ofVec2f offset(cos(angle), sin(angle));
  offset *= distance;
  double front = HeightMap::map->getHeightAt(position + offset);
  double back = HeightMap::map->getHeightAt(position - offset);
	double incline = atan2(front - back, 2 * distance / 200);

	return incline;
}

ofVec2f DepthPhysics::getCurveForces() {
	double weight = mass * gravity;
	double inclineX = getIncline(0, 2);
	double inclineY = getIncline(M_PI_2, 2);

	double fGravityX = weight *  sin(inclineX);
	double fGravityY = weight * sin(inclineY);

	double fNormalX = weight * cos(inclineX);
	double fNormalY = weight * cos(inclineY);

	double fFrictionX = fNormalX  * friction;
	fFrictionX = fabs(fFrictionX) * ((velocity.x < 0) ? 1 : -1);

	double fFrictionY = fNormalY * friction;
	fFrictionY = fabs(fFrictionY) * ((velocity.y < 0) ? 1 : -1);

	double fNetX = cos(inclineX) * (fGravityX + fFrictionX);
	double fNetY = cos(inclineY) * (fGravityY + fFrictionY);

  return ofVec2f(fNetX, fNetY);
}

ofVec2f DepthPhysics::getVelocityAtPoint(ofPoint position, double dt) {
  this->position = position;
  velocity +=  (getCurveForces() / mass) * dt;

	double angle = atan2(velocity.y, velocity.x);
	double incline = getIncline(angle, 1);
	return cos(incline) * velocity;
}
