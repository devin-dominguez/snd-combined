#include "gc_rgb.h"

void GCRgb::setup() {
  width = 320;
  height = 240;

  parameters.add(left.set("left", 0, 0, 160));
  parameters.add(right.set("right", 320, 161, 320));
  parameters.add(top.set("top", 0, 0, 120));
  parameters.add(bottom.set("bottom", 240, 121, 240));
  parameters.add(rotation.set("rotation", 0, -180, 180));

  calibration.setup("/dev/video0");
  setCalibration("White Balance, Automatic", 0);
  setCalibration("Gain, Automatic", 0);
  setCalibration("Power Line Frequency", 0);
  setCalibration("Auto Exposure", 0);

  displayPixels.allocate(width, height);
  camera.setDeviceID(0);
  camera.setDesiredFrameRate(30);
  camera.initGrabber(320, 240);
  resetSettings();
  resetCalibrationSettings();
  loadSettings();
  loadCalibrationSettings();
}

ofRectangle GCRgb::getBounds() {
  return bounds;
}

double GCRgb::getRotation() {
  return rotation;
}

void GCRgb::update() {
  camera.update();
}

void GCRgb::setCalibration(string param, double value) {
  if (calibration.parameters.get(param).type() == "11ofParameterIdE") {
    calibration.parameters.get(param).cast<double>().set(value);
  } else {
    calibration.parameters.get(param).cast<int>().set((int)value);
  }
  updateBounds();
}
void GCRgb::setParameter(string param, double value) {
  if (parameters.get(param).type() == "11ofParameterIdE") {
    parameters.get(param).cast<double>().set(value);
  } else {
    parameters.get(param).cast<int>().set((int)value);
  }
  updateBounds();
}

Json::Value GCRgb::getParams() {
  return paramsToJson(parameters);
}

Json::Value GCRgb::getCalibrationParams() {
  return paramsToJson(calibration.parameters);
}

Json::Value GCRgb::paramsToJson(ofParameterGroup paramGroup) {
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

ofPixels& GCRgb::getDisplayPixels() {
  if (camera.isFrameNew()) {
    ofPixels croppable = camera.getPixels();
    croppable.crop(bounds.x, bounds.y, bounds.width, bounds.height);
    displayPixels.setFromPixels(croppable);
    displayPixels.resize(width, height);
    displayPixels.rotate(rotation, width / 2.0, height / 2.0);
  }

  return displayPixels.getPixels();
}

ofPixels& GCRgb::getPixels() {
  return camera.getPixels();
}

void GCRgb::resetSettings() {
  settings.setValue("left", 0);
  settings.setValue("right", width);
  settings.setValue("top", 0);
  settings.setValue("bottom", height);
  settings.setValue("rotation", 0.0);

  applySettings();
}

void GCRgb::resetCalibrationSettings() {
  calibrationSettings.setValue("Brightness", 0);
  calibrationSettings.setValue("Contrast", 50);
  calibrationSettings.setValue("Exposure", 50);
  calibrationSettings.setValue("Gain", 0);
  calibrationSettings.setValue("Hue", 0);
  calibrationSettings.setValue("Saturation", 128);
  calibrationSettings.setValue("Sharpness", 0);

  applyCalibrationSettings();
}


void GCRgb::loadSettings() {
  settings.load("./rgb_settings.xml");
  applySettings();
}

void GCRgb::loadCalibrationSettings() {
  calibrationSettings.load("./rgb_calibration_settings.xml");
  applyCalibrationSettings();
}
void GCRgb::saveSettings() {
  syncSettings();
  settings.save("./rgb_settings.xml");
}

void GCRgb::saveCalibrationSettings() {
  syncCalibrationSettings();
  calibrationSettings.save("./rgb_calibration_settings.xml");
}

void GCRgb::applySettings() {
  left = settings.getValue("left", 0);
  right = settings.getValue("right", width);
  top = settings.getValue("top", 0);
  bottom = settings.getValue("bottom", height);
  rotation = settings.getValue("rotation", 0.0);

  updateBounds();
}

void GCRgb::applyCalibrationSettings() {
  setCalibration("Brightness", calibrationSettings.getValue("Brightness", 0));
  setCalibration("Contrast", calibrationSettings.getValue("Contrast", 50));
  setCalibration("Exposure", calibrationSettings.getValue("Exposure", 50));
  setCalibration("Gain", calibrationSettings.getValue("Gain", 0));
  setCalibration("Hue", calibrationSettings.getValue("Hue", 0));
  setCalibration("Saturation", calibrationSettings.getValue("Saturation", 128));
  setCalibration("Sharpness", calibrationSettings.getValue("Sharpness", 0));
}

void GCRgb::syncSettings() {
  settings.setValue("left", left);
  settings.setValue("right", right);
  settings.setValue("top", top);
  settings.setValue("bottom", bottom);
  settings.setValue("rotation", rotation);
}

void GCRgb::syncCalibrationSettings() {
  calibrationSettings.setValue(
    "Brightness",
    calibration.parameters.getInt("Brightness")
  );
  calibrationSettings.setValue(
    "Contrast",
    calibration.parameters.getInt("Contrast")
  );
  calibrationSettings.setValue(
    "Exposure",
    calibration.parameters.getInt("Exposure")
  );
  calibrationSettings.setValue(
    "Gain",
    calibration.parameters.getInt("Gain")
  );
  calibrationSettings.setValue(
    "Hue",
    calibration.parameters.getInt("Hue")
  );
  calibrationSettings.setValue(
    "Saturation",
    calibration.parameters.getInt("Saturation")
  );
  calibrationSettings.setValue(
    "Sharpness",
    calibration.parameters.getInt("Sharpness")
  );
}

void GCRgb::updateBounds() {
  bounds.y = top;
  bounds.x = left;
  bounds.height = bottom - top;
  bounds.width = right - left;
}
