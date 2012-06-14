#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	string filePrefix = "IMG_0008";
	
	ofFile lfpFile(ofToDataPath(filePrefix+".lfp", true));
	lytroFileTools.splitFile(lfpFile);
	
	ofFile stkFile(ofToDataPath(filePrefix+"-stk.lfp", true));
	lytroFileTools.splitFile(stkFile);
	
	viewer.setup(ofToDataPath("", true), filePrefix);
	//vector<ofFile> photoSets = libraryParser.getPhotoSets();
	//libraryParser.parsePhotoSets();
	/*
	string filePrefix = "IMG_0012";
	lytroFileTools.splitFile(filePrefix, "04");
	
	ofDirectory dataDirectory = ofToDataPath("04/", true);
	viewer.setup(dataDirectory, filePrefix);*/
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