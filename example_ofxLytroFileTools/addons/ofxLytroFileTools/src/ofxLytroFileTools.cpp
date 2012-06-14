/*
 *  ofxLytroFileTools.cpp
 *  ofxLytroFileToolsExample
 *
 *  Created by jason van cleave on 6/11/12.
 *
 *
 * Based off lfpsplitter
 * https://github.com/nrpatel/lfptools
 * 
 * Copyright (c) 2011, Nirav Patel <nrp@eclecti.cc>
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "ofxLytroFileTools.h"

#define LYTRO_SHA1_LENGTH 45
#define LYTRO_MAGIC_LENGTH 12
#define LYTRO_BLANK_LENGTH 35
#define LYTRO_STRING_LENGTH 256

ofxLytroFileTools::ofxLytroFileTools()
{

}
void ofxLytroFileTools::splitFile(ofFile file)
{
	fileName  = file.getBaseName();
	if (ofIsStringInString(fileName, "-stk")) {
		vector<string> parts = ofSplitString(fileName, "-stk");
		fileName = parts[0];
		cout << "fileName: " << fileName << endl;
	}
	//string fullPathToLFPFile = ofToDataPath(fileName + ".lfp", true);
	//string fullPathToSTKFile = ofToDataPath(fileName + "-stk.lfp", true);
	process(file.getAbsolutePath());
}

LFPSection* ofxLytroFileTools::parseSection(char **lfp, int *in_len)
{
    char *ptr = *lfp;
    int len = *in_len;
    LFPSection* section = new LFPSection();
    
    // There may be some null region between sections
    while (*ptr == '\0' && len)
	{
        ptr++;
        len--;
    }
    
    if (len <= LYTRO_MAGIC_LENGTH+sizeof(uint32_t)+LYTRO_SHA1_LENGTH+LYTRO_BLANK_LENGTH) 
	{
        delete section;
        return NULL;
    }
    
    // Copy the magic type
    memcpy(section->typecode, ptr, 4);
    // Move past the magic and the first 4 bytes of 0s
    ptr += LYTRO_MAGIC_LENGTH;
    len -= LYTRO_MAGIC_LENGTH;
    
    // the length is stored as a big endian unsigned 32 bit int
    section->len = ntohl(*(uint32_t *)ptr);
    ptr += sizeof(uint32_t);
    len -= sizeof(uint32_t);
    
    // copy and move past the sha1 string and the 35 byte empty space
    memcpy(section->sha1, ptr, LYTRO_SHA1_LENGTH);
    ptr += LYTRO_SHA1_LENGTH+LYTRO_BLANK_LENGTH;
    len -= LYTRO_SHA1_LENGTH+LYTRO_BLANK_LENGTH;
    
    // make sure there exists as much data as the header claims
    if (len < section->len)
	{
        delete section;
        return NULL;
    }
    
    // just directly reference the existing buffer
    section->data = ptr;
    
    ptr += section->len;
    len -= section->len;
    
    *lfp = ptr;
    *in_len = len;
    
    return section;
}

void ofxLytroFileTools::identifySection(LFPFile * lfp, LFPSection* section)
{
    char jpeg[10] = {0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46};
    char *ptr = NULL;
    int quotecount = 0;
    section->name = (char*)malloc(LYTRO_STRING_LENGTH);
	
    // Find the sha1 in the table of contents
    if ((ptr = strstr(lfp->table->data, section->sha1))) 
	{
        // Move backwards to the corresponding name
        while (quotecount < 3 && (ptr-- > lfp->table->data))
		{
			if (*ptr == '"') quotecount++;
		}
        
        // Read the name if we can
        if ((!quotecount == 3) || (sscanf(ptr, "\"%255[^\"]\"", section->name) != 1))
		{
			strcpy(section->name, "unknown");
		}
    }
    
    // Hard coded to assume that the 20x20 LUT is 1600 bytes
    if (section->len == 1600) 
	{
        section->type = LFP_DEPTH_LUT;
        strcpy(section->name, "depth");
        return;
    }
    
    // Check for the magic bytes to see if its a jpg
    if ((section->len > sizeof(jpeg)) && (memcmp(section->data, jpeg, sizeof(jpeg)) == 0)) 
	{
        section->type = LFP_JPEG;
        strcpy(section->name, "image");
        return;
    }
    
    // Assume anything that isn't called imageRef is plain text
    if (strcmp(section->name, "imageRef"))
	{
		section->type = LFP_TEXT;
	}
}

void ofxLytroFileTools::parseSections(LFPFile * lfp)
{
   	char magic[8] = {0x89, 0x4C, 0x46, 0x50, 0x0D, 0x0A, 0x1A, 0x0A};
	
    if (lfp->len > sizeof(magic))
	{
		if (memcmp(lfp->data, magic, sizeof(magic)) == 0) 
		{
			ofLogVerbose() << "header is present";
		}
		ofLogVerbose() << "FILE IS VALID";
	} else 
	{
		ofLogError() << "FILE IS INVALID";
	}
	
	char *ptr = lfp->data;
    int len = lfp->len;
    
    // Move past the first header
    ptr += LYTRO_MAGIC_LENGTH+sizeof(uint32_t);
    len -= LYTRO_MAGIC_LENGTH+sizeof(uint32_t);
    
    // Assume the first section is always the table of contents
    lfp->table = parseSection(&ptr, &len);
    lfp->table->type = LFP_TEXT;
	string table = "table";
    lfp->table->name = (char *)table.c_str();
    
    LFPSection* cur_section = NULL;
    while (len > 0) 
	{
        LFPSection* new_section = parseSection(&ptr, &len);
        if (!new_section) break;
        
        identifySection(lfp, new_section);
        
        if (!lfp->sections) lfp->sections = new_section;
        else if (cur_section) cur_section->next = new_section;
        cur_section = new_section;
    }
}



void ofxLytroFileTools::process(string path)
{
	ofFile file(path);
	
	if(!file.exists())
	{
		ofLogError("The file " + path + " is missing");
	} else 
	{
		ofBuffer buffer = ofBufferFromFile(path, true);
		LFPFile * lfp = new LFPFile ();
		
		lfp->len = buffer.size();
		lfp->data = buffer.getBinaryBuffer();
		lfp->filename = (char *)fileName.c_str();
		
		parseSections(lfp);
		saveSections(lfp);
	}
}


char * ofxLytroFileTools::getDepthString(const char *data, int *datalen, int len)
{
    // make sure there is enough space for the ascii formatted floats
    int filelen = 20*len/4;
    char *depth = new char[filelen];
    char *start = depth;
    int i = 0;
    
    depth[0] = '\0';
    
    for (i = 0; i < len/4; i++)
	{
        char val[20];
        int vallen = 0;
        snprintf(val, 20, "%f\n",*(float *)(data+i*4));
        vallen = strlen(val);
        strncpy(depth, val, vallen);
        depth += vallen;
    }
    
    *datalen = depth-start;
    
    return start;
}

char * ofxLytroFileTools::getConvertedImage(const unsigned char *data, int *datalen, int len)
{
    int filelen = 4*len/3;
    const unsigned char *ptr = data;
    unsigned short *image = new unsigned short[filelen*sizeof(short)];
    unsigned short *start = image;
    
    // Turn the 12 bits per pixel packed array into 16 bits per pixel
    // to make it easier to import into other libraries
    while (ptr < data+len)
	{
        *image++ = (*ptr << 8) | (*(ptr+1) & 0xF0);
        *image++ = ((*(ptr+1) & 0x0F) << 12) | (*(ptr+2) << 4);
        
        ptr += 3;
    }
    
    *datalen = filelen;
    
    return (char *)start;
}

void ofxLytroFileTools::saveData(const char *data, int len, const char *filename)
{
    ofBuffer buffer(data, len);
	bool didSave = false;
	
	string filePath = ofToDataPath(filename, true);
	didSave = ofBufferToFile(filePath, buffer, true);
	
	if (didSave)
	{
		ofLogVerbose() << filePath << " saved";
	}else 
	{
		ofLogError() << filePath << " was NOT saved";
	}

}

void ofxLytroFileTools::saveSections(LFPFile * lfp)
{
    char name[LYTRO_STRING_LENGTH];
    LFPSection* section = lfp->sections;
    int jpeg = 0, raw = 0, text = 0;
    char *buf;
    int buflen = 0;
    
    // Save the plaintext metadata
    snprintf(name, LYTRO_STRING_LENGTH, "%s_%s.txt", lfp->filename, lfp->table->name);
    saveData(lfp->table->data, lfp->table->len, name);
    
    while (section != NULL) 
	{
        switch (section->type) 
		{
            case LFP_RAW_IMAGE:
			{
				buf = getConvertedImage((unsigned char *)section->data, &buflen, section->len);
                if (buf) 
				{
                    snprintf(name, LYTRO_STRING_LENGTH, "%s_%s%d.raw", lfp->filename, section->name, raw++);
                    saveData(buf, buflen, name);
                    delete buf;
                }
                break;
			}
				
            case LFP_TEXT:
			{
				snprintf(name, LYTRO_STRING_LENGTH, "%s_%s%d.txt", lfp->filename, section->name, text++);
                saveData(section->data, section->len, name);
                break;
			}

            case LFP_DEPTH_LUT:
			{
				// Parse the depth lookup table and save as plaintext
                buf = getDepthString(section->data, &buflen, section->len);
                if (buf) 
				{
                    snprintf(name, LYTRO_STRING_LENGTH, "%s_%s.txt", lfp->filename, section->name);
                    saveData(buf, buflen, name);
                    delete buf;
                }
                break;
			}

            case LFP_JPEG:
			{
				snprintf(name, LYTRO_STRING_LENGTH, "%s_%.2d.jpg", lfp->filename, jpeg++);
                saveData(section->data, section->len, name);
                break;
			}
				
			default:
			{
				ofLogVerbose() << "saveSections DEFAULT THROWN";
			}
        }
        
        section = section->next;
    }
}

LFPFile* ofxLytroFileTools::createLFP(string fullPathToLFPFile)
{
	
	ofFile file(fullPathToLFPFile);
	
	if(!file.exists())
	{
		ofLogError("The file " + fullPathToLFPFile + " is missing");
	}
	
	ofBuffer buffer = ofBufferFromFile(fullPathToLFPFile, true);
	LFPFile * lfp = new LFPFile ();
	
	ofLogVerbose() << "buffer.size() " << buffer.size();
	
	lfp->len = buffer.size();
	lfp->data = buffer.getBinaryBuffer();
	return lfp;
}