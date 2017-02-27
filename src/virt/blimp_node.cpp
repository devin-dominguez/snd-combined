#include "blimp_node.h"
#include "blimp.h"
#include "projector.h"

BlimpNode::BlimpNode() {
  coolDownTime = 3.0;
  coolDown = 0.0;
  colliding = false;
  collided = false;
  ready = false;

  color = Entity::colors[2];

  numBlimps = 3;

  maxSize = 10.0;
  size = 0.0;
  sizeTarget = 0.0;
  rotation = 0.0;
  rotationRate = 360.0 / -6.0;
  lerpRate = 5.0;
}

void BlimpNode::setup(ofPoint nodePosition, double nodeSize) {
  this->nodePosition = nodePosition;
  this->nodeSize = nodeSize;
}

void BlimpNode::action(double dt) {
  cooling(dt);
  collisionAction();
  rotation += rotationRate * dt;

  if (ready) {
    size = ofLerp(size, maxSize, lerpRate * dt);
  } else {
    size = 0.0;
  }
}

void BlimpNode::collideWith(Entity* entity) {
  colliding = true;
}

void BlimpNode::cooling(double dt) {
  if (coolDown > 0.0) {
    coolDown -= dt;
    ready = false;
  } else {
    ready = true;
    coolDown = 0.0;
  }
}

void BlimpNode::collisionAction() {
  if (collided != colliding) {
    collided = colliding;
    if (collided && ready) {
      coolDown = coolDownTime;
      // spawn blimps
      for (size_t i = 0; i < numBlimps; i++) {
        double angle = (double)i / (double)numBlimps;
        angle += rotation / 360.0;
        angle *= M_PI * 2;
        ofPoint blimpPosition(cos(angle), sin(angle));
        blimpPosition *= nodeSize;
        blimpPosition += nodePosition;
        Blimp::create(blimpPosition, angle);
      }
    }
  }

  colliding = false;
}

void BlimpNode::draw(double fadeLevel) {
  double screenSize = Projector::getScale(nodeSize);
  double blimpScreenSize = Projector::getScale(size);

  ofPushStyle();
    ofEnableAlphaBlending();
    ofFill();
    ofSetColor(color, 255.0 * fadeLevel);
    for (size_t i = 0; i < numBlimps; i++) {
      double angle = 360.0 * (double)i / (double)numBlimps + rotation;
      ofPushMatrix();
      ofRotate(angle);
      ofTranslate(nodeSize * 0.75, 0);
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofDrawRectRounded(0, 0, blimpScreenSize * 2, blimpScreenSize, 10);
      ofPopMatrix();
    }
  ofPopStyle();
}

