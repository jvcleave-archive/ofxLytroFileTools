/*
 *  ofxLytroFileViewer.cpp
 *  ofxLFPToolsExample
 *
 *  Created by jason van cleave on 6/13/12.
 *  Copyright 2012 jasonvancleave.com. All rights reserved.
 *
 */

#include "ofxLytroFileViewer.h"

ofxLytroFileViewer::ofxLytroFileViewer()
{
	ofDisableArbTex();
	ofEnableNormalizedTexCoords();
	depthMax = -FLT_MAX;
	depthMin =  FLT_MAX;
	currentLayer = 0;
}

void ofxLytroFileViewer::setup(ofDirectory dataDirectory, string filePrefix)
{
	dataDirectory.listDir();
	vector<ofFile> files = dataDirectory.getFiles();
	
	for (int i = 0; i<files.size(); i++) 
	{
		if (files[i].getExtension() == "jpg") 
		{
			if (ofIsStringInString(files[i].getFileName(), filePrefix)) 
			{
				imagePaths.push_back(files[i].getFileName());
			}
		}
	}
	
	for (int i = 0; i<imagePaths.size(); i++) 
	{
		ofLogVerbose() << imagePaths[i];
		ofImage image;
		image.loadImage(imagePaths[i]);
		ofTexture texture = image.getTextureReference();
		textures.push_back(texture);
	}
	
	cout << filePrefix+"_depth.txt"  << endl;
	ofBuffer buffer = ofBufferFromFile(filePrefix+"_depth.txt", false);
	
	while (!buffer.isLastLine())
	{
		string line = buffer.getNextLine();
		floats.push_back(ofToFloat(line));
	}
	int x, y;
	
    for (y = 0; y < DEPTH_MAP_SIZE; y++) 
	{
        for (x = 0; x < DEPTH_MAP_SIZE; x++) 
		{
			int pos = (y * DEPTH_MAP_SIZE + x);
			depthMap[y][x] = floats[pos];
            if (depthMin > depthMap[y][x]) depthMin = depthMap[y][x];
            if (depthMax < depthMap[y][x]) depthMax = depthMap[y][x];
        }
	}
}

void ofxLytroFileViewer::update()
{
	
}

void ofxLytroFileViewer::draw()
{
	ofPushMatrix();
		glEnable(GL_DEPTH_TEST);
		
		int x, y;
		float w = 1.0 / (DEPTH_MAP_SIZE-1);
		float dw = 1.0 / (depthMax - depthMin);
		float ds = depthMin;
		
		textures[currentLayer].bind();
			glBegin(GL_QUADS);
			for (y = 0; y < DEPTH_MAP_SIZE-1; y++)
			{
				int yy = DEPTH_MAP_SIZE-1-y;
				for (x = 0; x < DEPTH_MAP_SIZE-1; x++)
				{
					glTexCoord2f((x + 0) * w, 1.0 - (y+0) * w); 
					glVertex3d(2.0*(x+0)*w - 1.0, 2.0*(y+0)*w - 1.0, -dw*(depthMap[yy+0][x+0]-ds));
					
					glTexCoord2f((x + 0) * w, 1.0 - (y+1) * w); 
					glVertex3d(2.0*(x+0)*w - 1.0, 2.0*(y+1)*w - 1.0, -dw*(depthMap[yy-1][x+0]-ds));
					
					glTexCoord2f((x + 1) * w, 1.0 - (y+1) * w); 
					glVertex3d(2.0*(x+1)*w - 1.0, 2.0*(y+1)*w - 1.0, -dw*(depthMap[yy-1][x+1]-ds));
					
					glTexCoord2f((x + 1) * w, 1.0 - (y+0) * w); 
					glVertex3d(2.0*(x+1)*w - 1.0, 2.0*(y+0)*w - 1.0, -dw*(depthMap[yy+0][x+1]-ds));
				}
			}
			glEnd();
		textures[currentLayer].unbind();
	ofPopMatrix();
}

void ofxLytroFileViewer::incrementLayer()
{
	if (currentLayer+1 < textures.size()) 
	{
		currentLayer++;
	}
}

void ofxLytroFileViewer::decrementLayer()
{
	if (currentLayer - 1 > 0)
	{
		currentLayer--;
	}
}

