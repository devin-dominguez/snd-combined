#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"
#include "ofxJSON.h"
#include "heightmap.h"

class GCDepth : public HeightMap {
  public:
    GCDepth();
    double getHeightAt(ofPoint position); // TODO
    void setWorldDimensions(double worldWidth, double worldHeight);
    void draw(double width, double height);

    void setup();
    void update();

    void setParameter(string param, double value);
    Json::Value getParams();

    ofPixels& getDisplayPixels();
    ofPixels& getPixels();

    void resetSettings();
    void loadSettings(); // TODO
    void saveSettings(); // TODO

  private:
    double worldWidth;
    double worldHeight;
    // Objects
    ofxKinect kinect;
    ofxCvGrayscaleImage displayPixels;
    ofRectangle bounds;
    ofxXmlSettings settings;
    ofParameterGroup parameters;
    // Methods
    void updateBounds();
    void applySettings();
    void syncSettings();
    Json::Value paramsToJson(ofParameterGroup paramGroup);
    // Data
    int width;
    int height;
    ofParameter<int> left;
    ofParameter<int> right;
    ofParameter<int> top;
    ofParameter<int> bottom;
    ofParameter<double> rotation;
    ofParameter<double> nearClip;
    ofParameter<double> farClip;

};

