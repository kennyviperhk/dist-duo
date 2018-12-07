//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//

#include "ofApp.h"


void ofApp::setup()
{
    serialSetup();
}

void ofApp::exit()
{
  //  for(int i=0; i< arduino.size(); i++){
    arduinoA.unregisterAllEvents(this);
    arduinoB.unregisterAllEvents(this);
  //  }
}

void ofApp::update(){

       receivedVal = serialUpdate();
       if(receivedVal[0] == 0){
           accelVal = receivedVal;
           }else{
        discVal = receivedVal;
               }
    
}

void ofApp::draw()
{
    serialDraw();
  
 
}
/*
string ofApp::serialRead(int a){
            
            string combinedStr = "";
            // for(int i=0; i< arduinoArranged.size(); i++){
            
            // The serial device can throw exeptions.
            
            try
            {
                // Read all bytes from the device;
                uint8_t buffer[1024];
                vector<uint8_t> finalBuffer;
                finalBuffer.clear();
                while (arduino[a].available() > 0)
                {
                    std::size_t sz = arduino[a].readBytes(buffer, 1024);
                    //ofLog() << "buffer size: " << sz;
                    for (std::size_t j = 0; j < sz; ++j)
                    {
                        // std::cout << buffer[j];
                        //ofLog() << "buf: " << buffer[j];
                        if(isalnum(buffer[j]) || buffer[j] == '|' || buffer[j] == '-' ){
                            finalBuffer.push_back(buffer[j]);
                        }
                        
                        
                    }
                    for(int i = 0; i< finalBuffer.size();i++){
                        //  ofLog() << "New Buf : " << finalBuffer[i];
                        combinedStr += ofToString(finalBuffer[i]);
                        //isReceivedString[a] = true;
                    }
                }
                
            }
            catch (const std::exception& exc)
            {
                ofLogError("ofApp::update") << exc.what();
            }
            return combinedStr;
        }
*/


vector<float> ofApp::serialUpdate(){
    
    vector<float> currVal;
    


    std::vector<SerialMessage>::iterator iter = serialMessages.begin();
    
    string receivedMsg = "";
    
    
    while (iter != serialMessages.end())
    {
        
        iter->fade -= 50;

        if (iter->fade < 0)
        {
            iter = serialMessages.erase(iter);  //may need this to maintain performance
        }
        else
        {
            
            /*
             ofSetColor(255, ofClamp(iter->fade, 0, 255));
             ofDrawBitmapString(iter->message, ofVec2f(x, y));
             
             y += height;
             
             */
            
            
            /*   if((iter->message).find('l') != std::string::npos){
             dir = "l";
             isNewDirData = true;
             }else if((iter->message).find('r') != std::string::npos){
             dir = "r";
             isNewDirData = true;
             }else if((iter->message).find('m') != std::string::npos){
             dir = "n";
             isNewDirData = false;
             // newDirCounter = ofGetElapsedTimef();
             }else{*/
            receivedMsg = iter->message;
            //  }
            
            
            
            if (!iter->exception.empty())
            {
                // y += height;
            }
            
            ++iter;
        }
    }
    
    bool isAccelVal = false;
    for(int i =0 ; i< receivedMsg.size(); i++){
        if(receivedMsg.find("euler") != std::string::npos){
            receivedMsg = receivedMsg.erase(0,5);
            receivedMsg = receivedMsg.erase(0,1);
            ofLog() << "euler:" << receivedMsg;
            
            isAccelVal = true;
        }else if(receivedMsg.find("hz") != std::string::npos){
            receivedMsg = receivedMsg.erase(0,2);
            //  ofLog() << "1. hz(0,2) " << receivedMsg;
            receivedMsg = receivedMsg.erase(0,1);
            ofLog() << "Hz:" << receivedMsg;
            isAccelVal = false;

        }
    }
    
    string s(receivedMsg);
    istringstream iss(s);
    if(isAccelVal){
        currVal.push_back(0);
    }else{
        currVal.push_back(1);
        }
    do
    {
        string sub;
        iss >> sub;
        
        currVal.push_back(ofToFloat(sub));
        //cout << "Substring: " << sub << endl;
        
    } while (iss);
    
    
    return currVal;
    
}


void ofApp::onSerialBuffer(const ofxIO::SerialBufferEventArgs& args)
{   
    ofLog() << args.buffer().toString();
    
    // Buffers will show up here when the marker character is found.
    SerialMessage message(args.buffer().toString(), "", 500);
    serialMessages.push_back(message);
 
}

void ofApp::onSerialError(const ofxIO::SerialBufferErrorEventArgs& args)
{
    // Errors and their corresponding buffer (if any) will show up here.
    SerialMessage message(args.buffer().toString(),
                          args.exception().displayText(),
                          500);
    serialMessages.push_back(message);
    ofLog() << "serialMessages onSerialError : " << serialMessages.size();
}

void ofApp::keyReleased(int key){
    switch(key){
    case 's':
    sendChar(1);
    break;    
        
    default:
    break;
    
    
    }


}



void ofApp::sendChar(int a){
    vector<uint8_t> hi;
    hi.push_back(a);
    ofx::IO::ByteBuffer buffer(hi);
    arduinoA.writeByte(hi[0]);
    arduinoB.writeByte(hi[0]);
    ofLog() << "char SEND";
}


void ofApp::serialSetup(){

    
    int a = 0;
    
    //================== Serial ==================
    
    vector<ofxIO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
    
    ofLogNotice("ofApp::setup") << "Connected Devices: ";
    
    for (std::size_t i = 0; i < devicesInfo.size(); ++i)
    {
        ofLogNotice("ofApp::setup") << "\t" << devicesInfo[i];
    }
    
    if (!devicesInfo.empty())
    {
        
        for (std::size_t i = 0; i < devicesInfo.size(); ++i)
        {
            string portDesc = devicesInfo[i].getHardwareId();
            ofLog() << "devicesInfo[i].getHardwareId() : " << devicesInfo[i].getHardwareId();
            bool success  = false;
            if(a == 0){
                success = arduinoA.setup(devicesInfo[i], 115200);
            }else if (a == 1){
                success = arduinoB.setup(devicesInfo[i], 115200);
            }
            
            
            if(success)
            {
                if(a == 0){
                    arduinoA.unregisterAllEvents(this);
                    arduinoA.registerAllEvents(this);
                }else if (a == 1){
                    arduinoB.unregisterAllEvents(this);
                    arduinoB.registerAllEvents(this);
                }
                
                ofLogNotice("ofApp::setup") << "Successfully setup " << devicesInfo[i];
                
                a++;
                
            }
            else
            {
                ofLogNotice("ofApp::setup") << "Unable to setup " << devicesInfo[i];
            }
            
        }
    }
    else
    {
        ofLogNotice("ofApp::setup") << "No devices connected.";
    }
    
}

void ofApp::serialDraw(){
    ofBackground(0);
    
    ofSetColor(255);
    
    std::stringstream ss;
    
    std::stringstream ss2;
    
    ss << "         FPS: " << ofGetFrameRate() << endl;
    
    // for(int i=0; i< arduino.size(); i++){
    ss << "Connected to: " << arduinoA.port()<< endl;
    ss << "Connected to: " << arduinoB.port()<< endl;
    
    for(int i = 0; i< accelVal.size(); i++){
        ss << "accelVal from Arduino: " << i << " : "<< accelVal[i] << endl;
    }
    
    for(int i = 0; i< discVal.size(); i++){
        ss2 << "discVal from Arduino: " << i << " : "<< discVal[i] << endl;
    }
    
    //  }
    
    ofDrawBitmapString(ss.str(), ofVec2f(20, 20));
    
    ofDrawBitmapString(ss2.str(), ofVec2f(20, 200));

}
