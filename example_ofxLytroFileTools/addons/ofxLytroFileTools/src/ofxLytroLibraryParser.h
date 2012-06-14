/*
 *  ofxLytroLibraryParser.h
 *  ofxLytroToolsExample
 *
 *  Created by jason van cleave on 6/13/12.
 *  Copyright 2012 jasonvancleave.com. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

class ofxLytroLibraryParser
{
public:
	ofxLytroLibraryParser();
	vector<ofFile> imageDirectoryContents;
	vector<ofFile> photoSets;
	vector<ofFile> getPhotoSets();
	void copyPhotoSetsToDataDirectory();
	ofDirectory lytroLib;
	void parsePhotoSets();
};

