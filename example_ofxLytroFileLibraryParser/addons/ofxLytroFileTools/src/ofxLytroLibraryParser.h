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

struct LytroLibraryItem 
{
	string photoSetName;
	string name;
	ofFile stk;
	ofFile lfp;
	string moveDestination;
	string finalSTKPath;
	string finalLFPPath;
	vector<ofFile> files;
	void move()
	{
		finalSTKPath = moveDestination+stk.getFileName();
		finalLFPPath = moveDestination+lfp.getFileName();
		stk.moveTo(finalSTKPath, false, true);
		lfp.moveTo(finalLFPPath, false, true);
	};
	
};

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
	void copyAndRipAll();
	vector<LytroLibraryItem> libraryItems;
};

