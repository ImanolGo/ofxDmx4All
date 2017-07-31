#include "ofxDmx4All.h"
#include "ofMain.h"


const int ofxDmx4All::BAUD_RATE = 38400;

ofxDmx4All::ofxDmx4All() :connected(false), needsUpdate(false) {
}

ofxDmx4All::~ofxDmx4All() {
	serial.close();
	connected = false;
}

bool ofxDmx4All::connect(int device, unsigned int channels) {
	serial.listDevices();
	connected = serial.setup(device, BAUD_RATE);
	setChannels(channels);
	return connected;
}

bool ofxDmx4All::connect(string device, unsigned int channels) {
	serial.listDevices();
	connected = serial.setup(device.c_str(), BAUD_RATE);
	setChannels(channels);
	return connected;
}

bool ofxDmx4All::autoconnect(unsigned int channels)
{
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    
    for(auto device: deviceList)
    {
        if(this->checkConnection(device.getDeviceID())) //open a device number
        {
            this->connect(device.getDeviceID(), channels);
            ofLogNotice() <<"Dmx4All connected to port " << device.getDeviceName();
			return true;
        }
    }

	return false;
}

bool ofxDmx4All::checkConnection(int portNum)
{
    if(serial.setup(portNum, BAUD_RATE)) //open a device number
    {
        this->sendPin();
        ofSleepMillis(50);
        if(this->receivedOk()){
            ofLogNotice() <<"Dmx4All connected to " << portNum;
			serial.close();
            return true;
        }
		else {
			serial.close();
		}
    }
    
    ofLogError() <<" Dmx4All is not connected ";
    //m_serial.setup(0, BAUD_RATE);
    return false;
    
}

void ofxDmx4All::sendPin()
{
    unsigned int packetSize = 2;
    vector<unsigned char> packet(packetSize);
    
    // Check Connection
    packet[0] = 'C';
    packet[1] = '?';
    
    serial.writeBytes(&packet[0], packetSize);
}

bool ofxDmx4All::receivedOk()
{
    if ( serial.available() == 0 )
    {
        return false;
    }
    
    /// // we want to read 1 bytes
    int bytesRequired = 1;
    unsigned char bytes[1];
    int bytesRemaining = bytesRequired;
    // loop until we've read everything
    while ( bytesRemaining > 0 ){
        // check for data
        if ( serial.available() > 0 ){
            // try to read - note offset into the bytes[] array, this is so
            // that we don't overwrite the bytes we already have
            int bytesArrayOffset = bytesRequired - bytesRemaining;
            int result = serial.readBytes( &bytes[bytesArrayOffset], bytesRemaining );
            
            // check for error code
            if ( result == OF_SERIAL_ERROR ){
                // something bad happened
                ofLog( OF_LOG_ERROR, "unrecoverable error reading from serial" );
                break;
            }
            else if ( result == OF_SERIAL_NO_DATA ){
                // nothing was read, try again
            }
            else {
                // we read some data!
                bytesRemaining -= result;
            }
        }
    }
    
    if(bytes[0] == 'G'){
        ofLogNotice() <<" Dmx4All checked connection with a G";
        return true;
    }
    
    return false;
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
