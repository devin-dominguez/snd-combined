#pragma once

#include "ofxHTTP.h"
#include "ofxJSONRPC.h"
#include "gc_rgb.h"
#include "gc_depth.h"
#include "color_tracker.h"

class CalibrationServer {
  public:
    void setup(
      GCRgb* rgbCamera,
      GCDepth* depthCamera,
      ColorTracker* colorTracker
    );
    void update();
    void setServerParam(ofx::JSONRPC::MethodArgs& args);
    void setRgbParam(ofx::JSONRPC::MethodArgs& args);
    void setRgbCalibrationParam(ofx::JSONRPC::MethodArgs& args);
    void setDepthParam(ofx::JSONRPC::MethodArgs& args);
    void setCvParam(ofx::JSONRPC::MethodArgs& args);
    void setProjectorParam(ofx::JSONRPC::MethodArgs& args);

  private:
    ofx::HTTP::JSONRPCServer jsonServer;
    ofx::HTTP::SimpleIPVideoServer imageServer;
    int context;
    int cvPixels;

    GCRgb* rgbCamera;
    GCDepth* depthCamera;
    ColorTracker* colorTracker;
};
