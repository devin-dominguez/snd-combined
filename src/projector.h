#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxJSON.h"


class Projector {
  public:
    static void setup(); // TODO
    static void startDraw();
    static void endDraw();

    static void drawBounds();

    static void setWorldDimensions(double width, double height);

    static void setSize(int dLeft, int dRight, int dTop, int dBottom);
    static void setRotation(double dRot);

    static ofPoint getCoords(ofPoint position);
    static double getScale(double size);

    static void setParameter(string param, double value);
    static Json::Value getParams();
    static void resetSettings();
    static void loadSettings(); // TODO
    static void saveSettings(); // TODO

  private:
    // Objects
    static ofPoint position;
    static ofRectangle bounds;
    // Methods
    static void updateBounds();
    static void drawMasking(); //TODO
    // Data
    static int width;
    static int height;
    static double worldWidth;
    static double worldHeight;

    static ofxXmlSettings settings;
    static ofParameterGroup parameters;
    // Methods
    static void applySettings();
    static void syncSettings();
    static Json::Value paramsToJson(ofParameterGroup paramGroup);
    // Data
    static ofParameter<double> x;
    static ofParameter<double> y;
    static ofParameter<double> left;
    static ofParameter<double> right;
    static ofParameter<double> top;
    static ofParameter<double> bottom;
    static ofParameter<double> rotation;
};
