#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	string filePrefix = "IMG_0008";
	lytroFileTools.splitFile(filePrefix);
	
	ofDirectory dataDirectory = ofToDataPath("", true);
	viewer.setup(dataDirectory, filePrefix);
}

//--------------------------------------------------------------
void testApp::update()
{
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(ofColor::white, ofColor::blue, OF_GRADIENT_CIRCULAR);
	
	camera.begin();
		ofPushMatrix();
			ofScale(50, 50, 50);
			viewer.draw();
		ofPopMatrix();
	camera.end();
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == '+')
	{
		viewer.incrementLayer();
		
	}
	
	if(key == '-')
	{
		viewer.decrementLayer();
		
	}
}