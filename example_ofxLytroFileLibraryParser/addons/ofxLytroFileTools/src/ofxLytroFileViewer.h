/*
 *  ofxLytroFileViewer.h
 *  ofxLFPToolsExample
 *
 *  Created by jason van cleave on 6/13/12.
 *
 */

#pragma once

#include "ofMain.h"


#define DEPTH_MAP_SIZE  (20)

class ofxLytroFileViewer
{
public:
	ofxLytroFileViewer();
	void setup(ofDirectory dataDirectory, string filePrefix);
	void update();
	void draw();
	int currentLayer;
	void incrementLayer();
	void decrementLayer();
private:
	vector<string> imagePaths;
	vector<ofTexture> textures;
	vector<float> floats;
	float depthMax;
	float depthMin;
	
	float depthMap[DEPTH_MAP_SIZE][DEPTH_MAP_SIZE];
};