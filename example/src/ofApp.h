#pragma once

#include "ofMain.h"
#include "ofxDmx4All.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofxDmx4All dmx;
	int level;
    
    ofxPanel panel;
    ofParameter<int> chan1;
    ofParameter<int> chan2;
    ofParameter<int> chan3;
    ofParameter<int> chan4;
    ofParameter<bool> autoCycle;
};
