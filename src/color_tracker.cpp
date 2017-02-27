#include "color_tracker.h"

// Lifecycle Methods
void ColorTracker::setup() {
  monoImage.allocate(320, 240);
  tempImage.allocate(320, 240);
  parameters.add(blobMinSize.set("blob_min_size", 0, 0, 10000));
  parameters.add(blobMaxSize.set("blob_max_size", 10000, 0, 10000));
  parameters.add(numBlobs.set("num_blobs", 4, 1, 8));
  parameters.add(colorBottom.set("color_bottom", 0, 0, 255));
  parameters.add(colorTop.set("color_top", 255, 0, 255));

  worldHeight = 640;
  worldWidth = 480;
  width = 320;
  width = 240;
  rotation = 0;
  bounds = ofRectangle(0, 0, 320, 240);

  resetSettings();
  loadSettings();
}

void ColorTracker::update(ofPixels* source, ofRectangle bounds, double rotation) {
  width = source->getWidth();
  height = source->getHeight();
  this->rotation = rotation;
  this->bounds = bounds;
  tempImage.setFromPixels(source->getData(), width, height);
  tempImage.convertRgbToHsv();
  tempImage.convertToGrayscalePlanarImage(monoImage, 0);
  size_t length = monoImage.getWidth() * monoImage.getHeight();
  ofPixels* pixels = &monoImage.getPixels();
  for (size_t i = 0; i < length; i++) {
    (*pixels)[i] = ofInRange(
                  (*pixels)[i],
                  colorBottom,
                  colorTop
                ) ? 255 : 0;
  }

  blobTracker.findContours(monoImage, blobMinSize, blobMaxSize, numBlobs, true);
}

Json::Value ColorTracker::getParams() {
  return paramsToJson(parameters);
}

Json::Value ColorTracker::paramsToJson(ofParameterGroup paramGroup) {
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

vector<ofVec2f> ColorTracker::getPoints() {
  vector<ofVec2f> points;
  for (size_t i = 0; i < blobTracker.nBlobs; i++) {
    ofVec2f point = blobTracker.blobs[i].centroid;
    if (bounds.inside(point)) {
      // translate to origin
      point -= ofVec2f(bounds.x, bounds.y);
      // normalize
      point /= ofVec2f(bounds.width, bounds.height);
      // convert to world
      point *= ofVec2f(worldWidth, worldHeight);
      // rptate around center
      point = point.getRotated(-rotation, ofVec2f(worldWidth / 2.0, worldHeight / 2.0));

      points.push_back(point);
    }
  }

  return points;
}

void ColorTracker::setWorldDimensions(double worldWidth, double worldHeight) {
  this->worldWidth = worldWidth;
  this->worldHeight = worldHeight;
}

void ColorTracker::setParameter(string param, double value) {
  if (parameters.get(param).type() == "11ofParameterIdE") {
    parameters.get(param).cast<double>().set(value);
  } else {
    parameters.get(param).cast<int>().set((int)value);
  }
  syncSettings();
}

ofPixels& ColorTracker::getDisplayPixels() {
  for (size_t i = 0; i < blobTracker.nBlobs; i++) {
    monoImage.drawBlobIntoMe(blobTracker.blobs[i], 128);
  }
  return monoImage.getPixels();
}

void ColorTracker::resetSettings() {
  settings.setValue("blob_min_size", 0);
  settings.setValue("blob_max_size", 10000);
  settings.setValue("num_blobs", 4);
  settings.setValue("color_bottom", 0);
  settings.setValue("color_top", 255);
  applySettings();
}
void ColorTracker::loadSettings() {
  settings.load("./color_tracker_settings.xml");
  applySettings();
}
void ColorTracker::saveSettings() {
  syncSettings();
  settings.save("./color_tracker_settings.xml");
}

void ColorTracker::applySettings() {
  blobMinSize = settings.getValue("blob_min_size", 0);
  blobMaxSize = settings.getValue("blob_max_size", 10000);
  numBlobs = settings.getValue("num_blobs", 4);
  colorBottom = settings.getValue("color_bottom", 0);
  colorTop = settings.getValue("color_top", 255);
}
void ColorTracker::syncSettings() {
  settings.setValue("blob_min_size", blobMinSize);
  settings.setValue("blob_max_size", blobMaxSize);
  settings.setValue("num_blobs", numBlobs);
  settings.setValue("color_bottom", colorBottom);
  settings.setValue("color_top", colorTop);
}
