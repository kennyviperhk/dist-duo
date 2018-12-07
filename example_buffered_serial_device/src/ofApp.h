

#pragma once


#include "ofMain.h"
#include "ofxSerial.h"


class SerialMessage
{
public:
    SerialMessage(): fade(0)
    {
    }

    SerialMessage(const std::string& _message,
                  const std::string& _exception,
                  int _fade):
        message(_message),
        exception(_exception),
        fade(_fade)
    {
    }

    std::string message;
    std::string exception;
    int fade;
};


class ofApp: public ofBaseApp
{
public:
    void setup() ;
    void update();
    void draw() ;
    void exit() ;
    
    void serialSetup();
    vector<float> serialUpdate();
    void serialDraw();
    
    void keyReleased(int key);

    void onSerialBuffer(const ofxIO::SerialBufferEventArgs& args);
    void onSerialError(const ofxIO::SerialBufferErrorEventArgs& args);


    vector<float> receivedVal;
    vector<float> accelVal;
    vector<float> discVal;

    
    ofxIO::BufferedSerialDevice arduinoA; // 0 Accel, 1 disc, 2 screen
    ofxIO::BufferedSerialDevice arduinoB; // 0 Accel, 1 disc, 2 screen

    std::vector<SerialMessage> serialMessages;
    
    void sendChar(int a);

};
