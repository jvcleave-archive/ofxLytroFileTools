#include "testApp.h"

int photoIndex = 0;
int maxPhotos = 0;
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	viewer = NULL;
	libraryParser.parsePhotoSets();
	libraryParser.copyAndRipAll();
	libraryItems = libraryParser.libraryItems;
	maxPhotos = libraryItems.size();
	for (int i=0; i<libraryItems.size(); i++) 
	{
		libraryItems[i].move();
		ofLogVerbose() << libraryItems[i].finalSTKPath;
		ofLogVerbose() << libraryItems[i].finalLFPPath;
		
		lytroFileTools.splitFile(ofFile(libraryItems[i].finalLFPPath));
		lytroFileTools.splitFile(ofFile(libraryItems[i].finalSTKPath));
	}
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
				if (viewer != NULL) 
				{
					viewer->draw();
				}
			//viewer.draw();
		ofPopMatrix();
	camera.end();
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == '+')
	{
		viewer->incrementLayer();
		
	}
	
	if(key == '-')
	{
		viewer->decrementLayer();
		
	}
	if(key == 'n')
	{
		if (photoIndex+1<maxPhotos) 
		{
			
			photoIndex++;
			createViewer();
		}
	}
	
}
void testApp::createViewer()
{
	
	viewer = NULL;
	delete viewer;
	viewer = new ofxLytroFileViewer();
	ofLogVerbose() << "libraryItems[photoIndex].name: " << libraryItems[photoIndex].name;
	viewer->setup(ofToDataPath("", true), libraryItems[photoIndex].name);
}