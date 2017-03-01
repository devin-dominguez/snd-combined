#include "ofApp.h"

#include "projector.h"
#include "fake_height.h"
#include "virt/virt.h"
#include "heightmap.h"
#include "fake_height.h"

void ofApp::setup() {
  ofSetFrameRate(60);
  ofSetLogLevel(OF_LOG_ERROR);
  screenWidth = 800;
  screenHeight = 600;
  //ofSetLogLevel(OF_LOG_VERBOSE);

  calibrating = getenv("CALIBRATE") ? true : false;
  testing = getenv("TESTING") ? true : false;
  if (!testing) {
    depthCamera.setup();
    rgbCamera.setup();
    colorTracker.setup();
    HeightMap::setHeightMap(&depthCamera);
    if (calibrating) {
      calibrationServer.setup(&rgbCamera, &depthCamera, &colorTracker);
    }
  } else {
    // fake data
    HeightMap::setHeightMap(new FakeHeight());
    //testBlobs.push_back(ofVec2f(60, 60));
    testBlobs.push_back(ofVec2f(600, 300));
    testBlobs.push_back(ofVec2f(400, 120));
    //testBlobs.push_back(ofVec2f(60, 400));
  }

  HeightMap::map->setWorldDimensions(640, 480);

  ofSetWindowShape(screenWidth, screenHeight);
  Projector::setup();
  Projector::setWorldDimensions(640, 480);
  Projector::setSize(0, screenWidth, 0, screenHeight);
  colorTracker.setWorldDimensions(640, 480);
  Virt::setup();
}

void ofApp::update() {
  double dt = ofGetLastFrameTime();
  if (!testing) {
    depthCamera.update();
    rgbCamera.update();
    colorTracker.update(
        &rgbCamera.getPixels(),
        rgbCamera.getBounds(),
        rgbCamera.getRotation()
        );

    if (calibrating) {
      calibrationServer.update();
    }
    Virt::update(dt, colorTracker.getPoints());
  } else {
    Virt::update(dt, testBlobs);
  }
}

void ofApp::draw() {
  ofBackground(50);
  Projector::startDraw();
  if (testing) {
    FakeHeight* fakeMap = (FakeHeight*)HeightMap::map;
    fakeMap->draw(640, 480);
  }
  //depthCamera.draw(640, 480);
  Virt::draw();
  Projector::endDraw();
}
