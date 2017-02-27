#include "calibration_server.h"
#include "projector.h"

void CalibrationServer::setup(
  GCRgb* rgbCamera,
  GCDepth* depthCamera,
  ColorTracker* colorTracker)
{
  this->rgbCamera = rgbCamera;
  this->depthCamera = depthCamera;
  this->colorTracker = colorTracker;
  context = 0;
  cvPixels = 0;

  ofx::HTTP::JSONRPCServerSettings jsonSettings;
  jsonSettings.setPort(8197);
  jsonServer.setup(jsonSettings);

  jsonServer.registerMethod("server", "server controls",
    this, &CalibrationServer::setServerParam);
  jsonServer.registerMethod("rgb", "rgb controls",
    this, &CalibrationServer::setRgbParam);
  jsonServer.registerMethod("rgb_calibration", "rgb_calibration controls",
    this, &CalibrationServer::setRgbCalibrationParam);
  jsonServer.registerMethod("depth", "depth controls",
    this, &CalibrationServer::setDepthParam);
  jsonServer.registerMethod("cv", "cv controls",
    this, &CalibrationServer::setCvParam);
  jsonServer.registerMethod("projector", "projector controls",
    this, &CalibrationServer::setProjectorParam);

  jsonServer.start();

  ofx::HTTP::SimpleIPVideoServerSettings imageSettings;
  imageSettings.setPort(7890);
  imageSettings.ipVideoRouteSettings.setMaxClientConnections(2);
  imageServer.setup(imageSettings);
  imageServer.start();
}

void CalibrationServer::update() {
  switch (context) {
    case 0:
      imageServer.send(rgbCamera->getDisplayPixels());
      break;
    case 1:
      imageServer.send(depthCamera->getDisplayPixels());
      break;
    case 2:
      switch (cvPixels) {
        case 0:
          imageServer.send(colorTracker->getDisplayPixels());
          break;
        case 1:
          imageServer.send(rgbCamera->getDisplayPixels());
          break;
      }
      break;
    default:
      break;
  }
}

void CalibrationServer::setServerParam(ofx::JSONRPC::MethodArgs& args) {
  string param = args.params[0].asString();
  if (param == "context") {
    context = args.params[1].asInt();
    return;
  }

  if (param == "cv_pixels") {
    cvPixels = args.params[1].asInt();
    return;
  }

  if (param == "sync") {
    Json::Value data(Json::objectValue);
    data["rgb"] = rgbCamera->getParams();
    data["depth"] = depthCamera->getParams();
    data["rgb_calibration"] = rgbCamera->getCalibrationParams();
    data["cv"] = colorTracker->getParams();
    data["projector"] = Projector::getParams();
    args.result = data;
    return;
  }
}

void CalibrationServer::setRgbParam(ofx::JSONRPC::MethodArgs& args) {
  string param = args.params[0].asString();
  if (param == "reset") {
    rgbCamera->resetSettings();
  } else if(param == "save") {
    rgbCamera->saveSettings();
  } else if(param == "load") {
    rgbCamera->loadSettings();
  } else {
    rgbCamera->setParameter(param, args.params[1].asFloat());
  }
}

void CalibrationServer::setDepthParam(ofx::JSONRPC::MethodArgs& args) {
  string param = args.params[0].asString();
  if (param == "reset") {
    depthCamera->resetSettings();
  } else if(param == "save") {
    depthCamera->saveSettings();
  } else if(param == "load") {
    depthCamera->loadSettings();
  } else {
    depthCamera->setParameter(param, args.params[1].asFloat());
  }
}

void CalibrationServer::setRgbCalibrationParam(ofx::JSONRPC::MethodArgs& args) {
  string param = args.params[0].asString();
  if (param == "reset") {
    rgbCamera->resetCalibrationSettings();
  } else if(param == "save") {
    rgbCamera->saveCalibrationSettings();
  } else if(param == "load") {
    rgbCamera->loadCalibrationSettings();
  } else {
    rgbCamera->setCalibration(param, args.params[1].asFloat());
  }
}

void CalibrationServer::setCvParam(ofx::JSONRPC::MethodArgs& args) {
  string param = args.params[0].asString();
  if (param == "reset") {
    colorTracker->resetSettings();
  } else if(param == "save") {
    colorTracker->saveSettings();
  } else if(param == "load") {
    colorTracker->loadSettings();
  } else {
    colorTracker->setParameter(param, args.params[1].asFloat());
  }
}

void CalibrationServer::setProjectorParam(ofx::JSONRPC::MethodArgs& args) {
  string param = args.params[0].asString();
  if (param == "reset") {
    Projector::resetSettings();
  } else if(param == "save") {
    Projector::saveSettings();
  } else if(param == "load") {
    Projector::loadSettings();
  } else {
    Projector::setParameter(param, args.params[1].asFloat());
  }
}
