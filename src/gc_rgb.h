#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxV4L2Settings.h"
#include "ofxXmlSettings.h"
#include "ofxJSON.h"

class GCRgb {
  public:
    void setup();
    void update();

    void setCalibration(string param, double value);
    void setParameter(string param, double value);

    ofRectangle getBounds();
    double getRotation();

    Json::Value getParams();
    Json::Value getCalibrationParams();

    ofPixels& getDisplayPixels();
    ofPixels& getPixels();


    void resetSettings();
    void loadSettings();
    void saveSettings();

    void resetCalibrationSettings();
    void loadCalibrationSettings();
    void saveCalibrationSettings();
  private:
    // Objects
    ofVideoGrabber camera;
    ofxV4L2Settings calibration;
    ofParameterGroup parameters;
    ofxCvColorImage displayPixels;
    ofxXmlSettings settings;
    ofxXmlSettings calibrationSettings;
    // Methods
    void updateBounds();
    void applySettings();
    void syncSettings();

    void applyCalibrationSettings();
    void syncCalibrationSettings();

    Json::Value paramsToJson(ofParameterGroup paramGroup);
    // Data
    int width;
    int height;
    ofRectangle bounds;
    ofParameter<int> left;
    ofParameter<int> right;
    ofParameter<int> top;
    ofParameter<int> bottom;
    ofParameter<double> rotation;
};
