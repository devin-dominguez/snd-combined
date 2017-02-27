#include "ofApp.h"
#include "ofAppNoWindow.h"


int main() {
  //ofAppNoWindow window;
	//ofSetupOpenGL(&window, 320, 240, OF_WINDOW);
	ofSetupOpenGL(1024, 768, OF_WINDOW);
	ofRunApp(new ofApp());
}
