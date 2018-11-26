#pragma once

#include "ofMain.h"
#include "ofxSerial.h"
//#define USE_PI
#define USE_GPIO



#ifdef USE_PI
#include "ofxOMXPlayer.h"
#else

#endif



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


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    vector<float> serialRead();
    void sendChar();
    
    void videoMixing();
    
    void onSerialBuffer(const ofx::IO::SerialBufferEventArgs& args);
    void onSerialError(const ofx::IO::SerialBufferErrorEventArgs& args);
    
    ofx::IO::BufferedSerialDevice deviceAccel;
    ofx::IO::BufferedSerialDevice deviceMotor;
    
    std::vector<SerialMessage> serialMessages;
    string receivedMsg;
    
    vector<float> receivedVal;
    
    
    
#ifdef USE_PI
    ofxOMXPlayer vid1;
    ofxOMXPlayer vid2;
#else
    ofVideoPlayer vid1;
    ofVideoPlayer vid2;
#endif
    
    float vidDisplayScale;
    
    bool debugMode;
    
    ofFbo finalVid;
    
    void sendDir(char a);
    void sendMoveTo(int b);
    long prevSwingTime;
    long prevAngleTime;
    float prevAngle;
    float currAngle;
    float angleChangeSpeed;
    long currTime;
    int currMoveTo;
    bool hasToMove;
    bool leftSide;
    int centerPoint;
    void initReset();
    
    
    int mixingMode;
    int numOfMixingMode;
    int vid1YPos;
    int vid2YPos;
    bool changeYPos;
    bool changeVid;
    int vidChannel;
    
    
    //=======DEBUG=====
    
    int serialFailCheck;
};




