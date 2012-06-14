/*
 *  ofxLytroLibraryParser.cpp
 *  ofxLytroToolsExample
 *
 *  Created by jason van cleave on 6/13/12.
 *  Copyright 2012 jasonvancleave.com. All rights reserved.
 *
 */

#include "ofxLytroLibraryParser.h"

ofxLytroLibraryParser::ofxLytroLibraryParser()
{
	ofDirectory lytroLibDir("~/Pictures/Lytro.lytrolib/images");
	lytroLib = lytroLibDir;
}

vector<ofFile> ofxLytroLibraryParser::getPhotoSets()
{
	
	lytroLib.listDir();
	imageDirectoryContents = lytroLib.getFiles();
	for(int i=0; i<imageDirectoryContents.size(); i++)
	{
		ofFile file(imageDirectoryContents[i].getAbsolutePath());
		ofLogVerbose() << "imageDirectoryContents[i].getAbsolutePath(): " << imageDirectoryContents[i].getAbsolutePath();
		if(file.isDirectory())
		{
			photoSets.push_back(file);
			ofLogVerbose() << "file.path(): " << file.path();
		}
		
	}
	return photoSets;
}

void ofxLytroLibraryParser::copyPhotoSetsToDataDirectory()
{
	if (photoSets.size() == 0)
	{
		getPhotoSets();
		
		if (photoSets.size() == 0) 
		{
			ofLogError() << "No photos found";
			return;
		}
	}
	ofDirectory photoSetsDirectory(ofToDataPath("photoSets", true));
	
	if (!photoSetsDirectory.exists()) 
	{
		photoSetsDirectory.create();
		ofLogVerbose() << photoSetsDirectory.path() << " created";
	}else 
	{
		ofLogVerbose() << photoSetsDirectory.path() << " exists";
	}
	
	lytroLib.copyTo(photoSetsDirectory.path(), false, true);
	
}

void ofxLytroLibraryParser::parsePhotoSets()
{
	copyPhotoSetsToDataDirectory();
	ofDirectory parsedPhotosDirectory(ofToDataPath("parsedPhotos", true));
	if (!parsedPhotosDirectory.exists()) 
	{
		parsedPhotosDirectory.create();
	}
	ofDirectory photoSetsDirectory(ofToDataPath("photoSets", true));
	photoSetsDirectory.listDir();
	vector<string> filesNamesToParse;
	for(int i=0; i<photoSetsDirectory.size(); i++)
	{
		ofFile file(photoSetsDirectory[i].path());
		if (file.isDirectory()) 
		{
			ofDirectory photoFolder(file.path());
			photoFolder.listDir();
			for (int j =0; j<photoFolder.size(); j++) 
			{
				ofFile photoFile(photoFolder[j]);
				if (ofIsStringInString(photoFile.getFileName(), ".lfp")) 
				{
					filesNamesToParse.push_back(photoFile.getBaseName());					
				}
			}
			
		}
	}
	
	for (int i=0; i<filesNamesToParse.size(); i++) 
	{
		ofLogVerbose() << "filesNamesToParse[i]: " << filesNamesToParse[i];
	}
	
}