#pragma once

#include "ofMain.h"

class Virt {
  public:
    static void setup();
    static void update(double dt, vector<ofVec2f> blobs);
    static void draw();
    static void createNodes(vector<ofVec2f> blobs);
    static bool nodeAtBlob(ofVec2f blob);
    static bool blobAtNode(ofVec2f node, vector<ofVec2f> blobs);
    static double maxBlobDistance;
};
