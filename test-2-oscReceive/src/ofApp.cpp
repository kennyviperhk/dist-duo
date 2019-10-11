#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60); // run at 60 fps
    ofSetVerticalSync(true);
    
    // listen on the given port
    ofLog() << "listening for osc messages on port " << PORT;
    
    /*OSC PIR*/
    receiver.setup(PORT);
    isHumanPresent = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    /*OSC PIR*/
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        // check for mouse moved message
        if(m.getAddress() == "/pir"){
            
            // both the arguments are floats
            receviedOSCString = m.getArgAsString(0);
            
            if(prevReceviedOSCString != receviedOSCString){
                if(receviedOSCString == "on"){
                    ofLog() << receviedOSCString << ", OSC is on";
                    isHumanPresent = true;
                    
                }else if(receviedOSCString == "off"){
                    ofLog() << receviedOSCString << ", OSC is off";
                    isHumanPresent = false;
                }
                prevReceviedOSCString = receviedOSCString;
            }
        }else{
            ofLog() << "other OSC msg";
        }
        
    }
    
    /*OSC PIR ends*/
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackgroundGradient(100, 0);
    
}



