

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
    void setup() override;
    void update();
    void draw() override;
    void exit() override;
    
    void keyReleased(int key);

    void onSerialBuffer(const ofxIO::SerialBufferEventArgs& args);
    void onSerialError(const ofxIO::SerialBufferErrorEventArgs& args);

    vector<float> serialRead();
    vector<float> receivedVal;
    vector<float> accelVal;
    vector<float> discVal;
    string receivedMsg;
    
    ofxIO::BufferedSerialDevice arduinoA; // 0 Accel, 1 disc, 2 screen
    ofxIO::BufferedSerialDevice arduinoB; // 0 Accel, 1 disc, 2 screen

    std::vector<SerialMessage> serialMessages;
    
    void sendChar(int a);

};
