#include "ofxDmx4All.h"
#include "ofMain.h"


ofxDmx4All::ofxDmx4All() :connected(false), needsUpdate(false) {
}

ofxDmx4All::~ofxDmx4All() {
	serial.close();
	connected = false;
}

bool ofxDmx4All::connect(int device, unsigned int channels) {
	serial.listDevices();
	connected = serial.setup(device, 38400); 
	setChannels(channels);
	return connected;
}

bool ofxDmx4All::connect(string device, unsigned int channels) {
	serial.listDevices();
	connected = serial.setup(device.c_str(), 38400);
	setChannels(channels);
	return connected;
}

bool ofxDmx4All::isConnected() {
	return connected;
}

void ofxDmx4All::disconnect() {
	serial.close();
    connected = false;
}

void ofxDmx4All::setChannels(unsigned int channels) {
	levels.resize(ofClamp(channels, 24, 512));
}



void ofxDmx4All::update(bool force) {
	if(needsUpdate || force) {
		needsUpdate = false;

		unsigned int packetSize = levels.size()*3;
		vector<unsigned char> packet(packetSize);
		
		for(int i = 0; i<levels.size();i++){
			packet[i * 3 + 0] = (i < 256) ? 0xE2 : 0xE3; 
			packet[i * 3 + 1] = i; 
			packet[i * 3 + 2] = levels[i]; 
		}
		
		serial.writeBytes(&packet[0], packetSize);
	}
}

bool ofxDmx4All::badChannel(unsigned int channel) {
	if(channel > levels.size()) {
		ofLogError() << "Channel " + ofToString(channel) + " is out of bounds. Only " + ofToString(levels.size()) + " channels are available.";
		return true;
	}
	if(channel == 0) {
		ofLogError() << "Channel 0 does not exist. DMX channels start at 1.";
		return true;
	}
	return false;
}

void ofxDmx4All::setLevel(unsigned int channel, unsigned char level, unsigned int universe) {
	if(badChannel(channel)) {
		return;
	}
	channel--; // convert from 1-initial to 0-initial
    if(level != levels[channel]) {
        levels[channel] = level;
        needsUpdate = true;
    }
}

void ofxDmx4All::clear() {
	for (int i = 0; i < levels.size(); i++) {
		levels[i] = 0;
	}
}

unsigned char ofxDmx4All::getLevel(unsigned int channel) {
	if(badChannel(channel)) {
		return 0;
	}
	channel--; // convert from 1-initial to 0-initial
	return levels[channel];
}
