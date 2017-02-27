#pragma once

#include "ofMain.h"
#include "calibration_server.h"
#include "gc_rgb.h"
#include "gc_depth.h"
#include "color_tracker.h"

class ofApp: public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    CalibrationServer calibrationServer;
    GCRgb rgbCamera;
    GCDepth depthCamera;
    ColorTracker colorTracker;
    int screenWidth;
    int screenHeight;

    bool calibrating;
};
