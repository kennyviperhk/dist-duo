#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

// listening port
#define PORT 9001




class ofApp : public ofBaseApp{
public:
    
    void setup();
    void update();
    void draw();
    
    /*OSC PIR*/
    ofxOscReceiver receiver;
    string receviedOSCString;
    string prevReceviedOSCString;
    bool isHumanPresent;
};

