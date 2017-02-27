#include "rocket_node.h"
#include "rocket.h"
#include "projector.h"

RocketNode::RocketNode() {
  coolDownTime = 3.0;
  coolDown = 0.0;
  colliding = false;
  collided = false;
  ready = false;

  color = Entity::colors[2];

  numRockets = 9;

  maxSize = 10.0;
  size = 0.0;
  sizeTarget = 0.0;
  rotation = 0.0;
  rotationRate = 360.0 / -6.0;
  lerpRate = 3.0;
}

void RocketNode::setup(ofPoint nodePosition, double nodeSize) {
  this->nodePosition = nodePosition;
  this->nodeSize = nodeSize;
}

void RocketNode::action(double dt) {
  cooling(dt);
  collisionAction();
  rotation += rotationRate * dt;

  if (ready) {
    size = ofLerp(size, maxSize, lerpRate * dt);
  } else {
    size = 0.0;
  }
}

void RocketNode::collideWith(Entity* entity) {
  colliding = true;
}

void RocketNode::cooling(double dt) {
  if (coolDown > 0.0) {
    coolDown -= dt;
    ready = false;
  } else {
    ready = true;
    coolDown = 0.0;
  }
}

void RocketNode::collisionAction() {
  if (collided != colliding) {
    collided = colliding;
    if (collided && ready) {
      coolDown = coolDownTime;
      // spawn rockets
      for (size_t i = 0; i < numRockets; i++) {
        double angle = (double)i / (double)numRockets;
        angle += rotation / 360.0;
        angle *= M_PI * 2;
        ofPoint rocketPosition(cos(angle), sin(angle));
        rocketPosition *= nodeSize;
        rocketPosition += nodePosition;
        Rocket::create(rocketPosition, angle);
      }
    }
  }

  colliding = false;
}

void RocketNode::draw(double fadeLevel) {
  double screenSize = Projector::getScale(size);
  double rocketScreenSize = Projector::getScale(size);

  ofPushStyle();
    ofEnableAlphaBlending();
    ofFill();
    ofSetColor(color, 255.0 * fadeLevel);
    for (size_t i = 0; i < numRockets; i++) {
      double angle = 360.0 * (double)i / (double)numRockets + rotation;
      ofPushMatrix();
      ofRotate(angle);
      ofTranslate(nodeSize, 0);
        ofBeginShape();
          ofVertex(rocketScreenSize, 0);
          ofVertex(-rocketScreenSize, rocketScreenSize / 2.0);
          ofVertex(0, 0);
          ofVertex(-rocketScreenSize, -rocketScreenSize / 2.0);
          ofVertex(rocketScreenSize, 0);
        ofEndShape();
      ofPopMatrix();
    }
  ofPopStyle();
}

