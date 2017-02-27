#include "ofApp.h"

#include "projector.h"
#include "fake_height.h"
#include "virt/virt.h"
#include "heightmap.h"
#include "fake_height.h"

void ofApp::setup() {
  calibrating = getenv("CALIBRATE") ? true : false;
  ofSetFrameRate(60);
  ofSetLogLevel(OF_LOG_ERROR);
  //screenWidth = ofGetScreenWidth();
  //screenHeight = ofGetScreenHeight();
  screenWidth = 800;
  screenHeight = 600;
  //ofSetLogLevel(OF_LOG_VERBOSE);
  depthCamera.setup();
  rgbCamera.setup();
  colorTracker.setup();
  if (calibrating) {
    calibrationServer.setup(&rgbCamera, &depthCamera, &colorTracker);
  }

  //HeightMap::setHeightMap(new FakeHeight());
  HeightMap::setHeightMap(&depthCamera);
  HeightMap::map->setWorldDimensions(640, 480);

  ofSetWindowShape(screenWidth, screenHeight);
  Projector::setup();
  Projector::setWorldDimensions(640, 480);
  Projector::setSize(0, screenWidth, 0, screenHeight);
  colorTracker.setWorldDimensions(640, 480);
  Virt::setup();
}

void ofApp::update() {
  depthCamera.update();
  rgbCamera.update();
  colorTracker.update(&rgbCamera.getPixels(), rgbCamera.getBounds(), rgbCamera.getRotation());
  if (calibrating) {
    calibrationServer.update();
  }
  double dt = ofGetLastFrameTime();
  Virt::update(dt, colorTracker.getPoints());
}

void ofApp::draw() {
  ofBackground(50);
  Projector::startDraw();
  //depthCamera.draw(640, 480);
  Virt::draw();
  Projector::endDraw();
}
