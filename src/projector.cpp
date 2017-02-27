#include "projector.h"

// Data
double Projector::worldWidth = 640;
double Projector::worldHeight = 480;
int Projector::width = 800;
int Projector::height = 600;

ofxXmlSettings Projector::settings;
ofParameterGroup Projector::parameters;
ofParameter<double> Projector::left = 0;
ofParameter<double> Projector::right = Projector::width;
ofParameter<double> Projector::top = 0;
ofParameter<double> Projector::bottom = Projector::height;
ofParameter<double> Projector::rotation = 0;
ofParameter<double> Projector::x = 0;
ofParameter<double> Projector::y = 0;

// Objects
ofPoint Projector::position = ofPoint(Projector::x, Projector::y);
ofRectangle Projector::bounds = ofRectangle(
  0, 0, Projector::width, Projector::height
);


// Lifecycle Methods
void Projector::setup() {
  parameters.add(left.set("left", 0, 0, 400));
  parameters.add(right.set("right", 800, 401, 800));
  parameters.add(top.set("top", 0, 0, 300));
  parameters.add(bottom.set("bottom", 600, 301, 600));
  parameters.add(rotation.set("rotation", 0, -180, 180));
  parameters.add(x.set("x", 0, 0, 400));
  parameters.add(y.set("y", 0, 0, 300));

  resetSettings();
  loadSettings();
}

Json::Value Projector::getParams() {
  return paramsToJson(parameters);
}

Json::Value Projector::paramsToJson(ofParameterGroup paramGroup) {
  Json::Value data(Json::objectValue);
  for (size_t i = 0; i < paramGroup.size(); i++) {
    Json::Value param(Json::objectValue);
    if (paramGroup.getType(i) == "11ofParameterIdE") {
      ofParameter<double> p = paramGroup.get(i).cast<double>();
      param["min"] = p.getMin();
      param["max"] = p.getMax();
      param["value"] = p.get();
      data[p.getName()] = param;
    } else {
      ofParameter<int> p = paramGroup.get(i).cast<int>();
      param["min"] = p.getMin();
      param["max"] = p.getMax();
      param["value"] = p.get();
      data[p.getName()] = param;
    }
  }

  return data;
}

void Projector::setParameter(string param, double value) {
  if (parameters.get(param).type() == "11ofParameterIdE") {
    parameters.get(param).cast<double>().set(value);
  } else {
    parameters.get(param).cast<int>().set((int)value);
  }
  updateBounds();
  syncSettings();
}

void Projector::resetSettings() {
  settings.setValue("left", 0);
  settings.setValue("right", width);
  settings.setValue("top", 0);
  settings.setValue("bottom", height);
  settings.setValue("rotation", 0.0);
  settings.setValue("x", 0.0);
  settings.setValue("y", 0.0);

  applySettings();
}
void Projector::loadSettings() {
  settings.load("./projector_settings.xml");
  applySettings();
}
void Projector::saveSettings() {
  syncSettings();
  settings.save("./projector_settings.xml");
}

void Projector::applySettings() {
  left = settings.getValue("left", 0);
  right = settings.getValue("right", width);
  top = settings.getValue("top", 0);
  bottom = settings.getValue("bottom", height);
  rotation = settings.getValue("rotation", 0.0);
  x = settings.getValue("x", 0.0);
  y = settings.getValue("y", 0.0);

  updateBounds();
}
void Projector::syncSettings() {
  settings.setValue("left", left);
  settings.setValue("right", right);
  settings.setValue("top", top);
  settings.setValue("bottom", bottom);
  settings.setValue("rotation", rotation);
  settings.setValue("x", x);
  settings.setValue("y", y);
}

void Projector::startDraw() {
    ofPushMatrix();
      ofTranslate(position + bounds.getCenter());
      ofRotate(rotation);
}

void Projector::endDraw() {
      drawMasking();
  ofPopMatrix();
}

// Data Interface

void Projector::setWorldDimensions(double width, double height) {
  worldWidth = width;
  worldHeight = height;
}

void Projector::setSize(int dLeft, int dRight, int dTop, int dBottom) {
  left = dLeft;
  right = dRight;
  top = dTop;
  bottom = dBottom;
  updateBounds();
}

void Projector::setRotation(double dRot) {
  rotation = dRot;
}

void Projector::drawBounds() {
  ofPushStyle();

  ofNoFill();
  ofDrawRectangle(-bounds.getCenter(), bounds.width, bounds.height);
  ofDrawCircle(0, 0, 10);
  ofPopStyle();
}

ofPoint Projector::getCoords(ofPoint position) {
  double ratioX = bounds.width / worldWidth;
  double ratioY = bounds.height / worldHeight;
  double x = ratioX * position.x + bounds.x;
  double y = ratioY * position.y + bounds.y;

  return ofPoint(x, y) - bounds.getCenter();
}

double Projector::getScale(double size) {
  double ratioX = bounds.width / worldWidth;
  double ratioY = bounds.height / worldHeight;

  return size * fmax(ratioX, ratioY);
}

// Internal Methods
void Projector::drawMasking() {
  ofPoint offset = bounds.getCenter();
  ofPushStyle();
  ofSetColor(0);
  ofFill();
  ofDrawRectangle(ofPoint(-25000, 0) - offset, 50000, -25000);
  ofDrawRectangle(ofPoint(-25000, bounds.height) - offset, 50000, 25000);
  ofDrawRectangle(ofPoint(0, -25000) - offset, -25000, 50000);
  ofDrawRectangle(ofPoint(bounds.width, -25000) - offset, 25000, 50000);

  ofPopStyle();
}

void Projector::updateBounds() {
  bounds.y = top;
  bounds.x = left;
  bounds.height = bottom - top;
  bounds.width = right - left;
  position.x = x;
  position.y = y;
}
