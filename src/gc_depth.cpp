#include "gc_depth.h"
#include "projector.h"

// Data

GCDepth::GCDepth() {
  width = 640;
  height = 480;
  worldWidth = 640;
  worldHeight = 480;
}

// Lifecycle Methods
void GCDepth::setup() {
  parameters.add(left.set("left", 0, 0, 320));
  parameters.add(right.set("right", 640, 321, 640));
  parameters.add(top.set("top", 0, 0, 240));
  parameters.add(bottom.set("bottom", 480, 241, 480));
  parameters.add(rotation.set("rotation", 0, -180, 180));
  parameters.add(nearClip.set("near_clip", 50, 50, 4000));
  parameters.add(farClip.set("far_clip", 4000, 50, 4000));


  kinect.setRegistration(false);
  kinect.init(false, false);
  kinect.open();
  displayPixels.allocate(width, height);
  resetSettings();
  loadSettings();
}

void GCDepth::update() {
  kinect.update();
}

Json::Value GCDepth::getParams() {
  return paramsToJson(parameters);
}

Json::Value GCDepth::paramsToJson(ofParameterGroup paramGroup) {
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

void GCDepth::setParameter(string param, double value) {
  if (parameters.get(param).type() == "11ofParameterIdE") {
    parameters.get(param).cast<double>().set(value);
  } else {
    parameters.get(param).cast<int>().set((int)value);
  }
  updateBounds();
  syncSettings();
}

void GCDepth::setWorldDimensions(double worldWidth, double worldHeight) {
  this->worldWidth = worldWidth;
  this->worldHeight = worldHeight;
}

double GCDepth::getHeightAt(ofPoint position) { // TODO
  ofVec2f pos = position;
  //normalize
  pos /= ofPoint(worldWidth, worldHeight);
  // tanslate and clip
  pos.x *= bounds.width;
  pos.y *= bounds.height;
  pos.x += bounds.x;
  pos.y += bounds.y;
  // rotate
  pos = pos.getRotated(rotation, bounds.getCenter());

  double w = width;
  double h = height;
  double x1 = (int)pos.x;
  double x2 = x1 + 1;
  double y1 = (int)pos.y;
  double y2 = y1 + 1;
  
  //clip to heightmap size to avoid potential disaster
  x1 = ofClamp(x1, 0, w - 1);
  x2 = ofClamp(x2, 0, w - 1);
  y1 = ofClamp(y1, 0, h - 1);
  y2 = ofClamp(y2, 0, h - 1);
  
  //lookup height values
  double z11 = kinect.getDepthPixels()[x1 + y1 * w];
  double z12 = kinect.getDepthPixels()[x1 + y2 * w];
  double z21 = kinect.getDepthPixels()[x2 + y1 * w];
  double z22 = kinect.getDepthPixels()[x2 + y2 * w];
  
  //interpolate
  double r1 = (x2 - pos.x) * z11 + (pos.x - x1) * z21;
  double r2 = (x2 - pos.x) * z12 + (pos.x - x1) * z22;
  double z = (y2 - pos.y) * r1 + (pos.y - y1) * r2;
  
  return z / 255.0;
}

void GCDepth::draw(double width, double height) {
  ofImage draw;
  draw.setFromPixels(getDisplayPixels().getData(), 640, 480, OF_IMAGE_GRAYSCALE);
  ofPoint position = Projector::getCoords(ofPoint(0, 0));
  double screenWidth = Projector::getScale(640);
  double screenHeight = Projector::getScale(480);
  draw.draw(position.x, position.y, screenWidth, screenHeight);
}

ofPixels& GCDepth::getDisplayPixels() {
  if (kinect.isFrameNew()) {
    ofPixels croppable = kinect.getDepthPixels();
    croppable.crop(bounds.x, bounds.y, bounds.width, bounds.height);
    displayPixels.setFromPixels(
      croppable.getData(),
      croppable.getWidth(),
      croppable.getHeight()
    );
    displayPixels.resize(width, height);
    displayPixels.rotate(rotation, width / 2.0, height / 2.0);
  }

  return displayPixels.getPixels();
}

ofPixels& GCDepth::getPixels() {
  return kinect.getDepthPixels();
}
// Settings Interface
void GCDepth::resetSettings() {
  settings.setValue("left", 0);
  settings.setValue("right", width);
  settings.setValue("top", 0);
  settings.setValue("bottom", height);
  settings.setValue("rotation", 0.0);
  settings.setValue("near_clip", 50.0);
  settings.setValue("far_clip", 4000.0);
  applySettings();
}
void GCDepth::loadSettings() {
  settings.load("./depth_settings.xml");
  applySettings();
}
void GCDepth::saveSettings() {
  syncSettings();
  settings.save("./depth_settings.xml");
}

// Internal Methods
void GCDepth::applySettings() {
  left = settings.getValue("left", 0);
  right = settings.getValue("right", width);
  top = settings.getValue("top", 0);
  bottom = settings.getValue("bottom", height);
  rotation = settings.getValue("rotation", 0.0);
  nearClip = settings.getValue("near_clip", 50.0);
  farClip = settings.getValue("far_clip", 4000.0);

  updateBounds();
}
void GCDepth::syncSettings() {
  settings.setValue("left", left);
  settings.setValue("right", right);
  settings.setValue("top", top);
  settings.setValue("bottom", bottom);
  settings.setValue("rotation", rotation);
  settings.setValue("near_clip", nearClip);
  settings.setValue("far_clip", farClip);
}

void GCDepth::updateBounds() {
  kinect.setDepthClipping(nearClip, farClip);
  bounds.y = top;
  bounds.x = left;
  bounds.height = bottom - top;
  bounds.width = right - left;
}
