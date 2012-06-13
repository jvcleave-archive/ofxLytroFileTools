#pragma once

#include "ofMain.h"
#include "ofxLytroFileTools.h"
#include "ofxLytroFileViewer.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);

	ofxLytroFileTools lytroFileTools;
	ofxLytroFileViewer viewer;
	ofEasyCam camera;

};
