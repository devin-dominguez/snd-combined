#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxJSON.h"

class ColorTracker {
  public:
    void setup();
    void update(ofPixels* source, ofRectangle bounds, double rotation);
    void setWorldDimensions(double worldWidth, double worldHeight);

    vector<ofVec2f> getPoints();

    void setParameter(string param, double value);
    Json::Value getParams();

    double getDepthAt(double x, double y); // TODO
    ofPixels& getDisplayPixels();

    void resetSettings();
    void loadSettings();
    void saveSettings();

  private:
    double width;
    double height;
    double worldWidth;
    double worldHeight;
    ofRectangle bounds;
    double rotation;
    // Objects
    ofPixels* source;
    ofxCvColorImage tempImage;
    ofxCvGrayscaleImage monoImage;
    ofxXmlSettings settings;
    ofParameterGroup parameters;
    ofxCvContourFinder blobTracker;
    // Methods
    void applySettings();
    void syncSettings();
    Json::Value paramsToJson(ofParameterGroup paramGroup);
    // Data
    ofParameter<int> blobMinSize;
    ofParameter<int> blobMaxSize;
    ofParameter<int> numBlobs;
    ofParameter<int> colorBottom;
    ofParameter<int> colorTop;



};

