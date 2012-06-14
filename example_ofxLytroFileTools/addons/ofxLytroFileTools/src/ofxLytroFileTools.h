/*
 *  ofxLytroFileTools.h
 *  ofxLytroFileToolsExample
 *
 *  Created by jason van cleave on 6/11/12.
 *
 */
#pragma once

#include "ofMain.h"

typedef enum {
    LFP_RAW_IMAGE,
    LFP_TEXT,
    LFP_DEPTH_LUT,
    LFP_JPEG
} section_type;

struct LFPSection {
    section_type type;
    char typecode[4];
    int len;
    char sha1[46]; // including null termination
    char *name;
    char *data;
    struct LFPSection *next;
};

struct LFPFile  {
    char *filename;
    char *data;
    int len;
    LFPSection* table;
    LFPSection* sections;
};



class ofxLytroFileTools
{
public:
	ofxLytroFileTools();
	string fileName;
	void splitFile(ofFile file);
	
private:
	LFPFile * createLFP(string fullPathToLFPFile);
	LFPSection* parseSection(char **lfp, int *in_len);
	void identifySection(LFPFile * lfp, LFPSection* section);
	void saveSections(LFPFile * lfp);
	void parseSections(LFPFile * lfp);
	void process(string path);
	void saveData(const char *data, int len, const char *filename);
	char * getConvertedImage(const unsigned char *data, int *datalen, int len);
	char * getDepthString(const char *data, int *datalen, int len);

};
