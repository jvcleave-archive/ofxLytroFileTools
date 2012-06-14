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
void ofxLytroLibraryParser::copyAndRipAll()
{
	ofDirectory parsedPhotosDirectory(ofToDataPath("parsedPhotos", true));
	if (!parsedPhotosDirectory.exists()) 
	{
		parsedPhotosDirectory.create();
	}
	cout << "libraryItems.size(): " << libraryItems.size() << endl;
	for (int i=0; i<libraryItems.size(); i++) 
	{
		for (int j=0; j<libraryItems[i].files.size(); j++) 
		{
			if (ofIsStringInString(libraryItems[i].files[j].getFileName(), "-stk") )
			{
				libraryItems[i].stk = libraryItems[i].files[j];
			}else
			{
				libraryItems[i].lfp = libraryItems[i].files[j];
				libraryItems[i].name = libraryItems[i].photoSetName+"_"+libraryItems[i].files[j].getBaseName();
			}
			libraryItems[i].moveDestination = parsedPhotosDirectory.path()+"/"+libraryItems[i].photoSetName+"_";
			//libraryItems[i].move();
		}
	}
}
void ofxLytroLibraryParser::parsePhotoSets()
{
	copyPhotoSetsToDataDirectory();
	
	ofDirectory photoSetsDirectory(ofToDataPath("photoSets", true));
	photoSetsDirectory.listDir();
	
	for(int i=0; i<photoSetsDirectory.size(); i++)
	{
		ofFile file(photoSetsDirectory[i].path());
		if (file.isDirectory()) 
		{
			LytroLibraryItem libraryItem;
			libraryItem.photoSetName = file.getBaseName();
			
			ofDirectory photoFolder(file.path());
			photoFolder.listDir();
			for (int j =0; j<photoFolder.size(); j++) 
			{
				ofFile photoFile(photoFolder[j]);
				if (ofIsStringInString(photoFile.getFileName(), ".lfp")) 
				{
					libraryItem.files.push_back(photoFile);
					libraryItems.push_back(libraryItem);
					//filesNamesToParse.push_back(photoFile.getBaseName());
					//photoFile.moveTo(parsedPhotosDirectory.path()+"/"+file.getBaseName()+"_"+photoFile.getFileName(), false, false);
				}
			}
			
		}
	}
	
	
}