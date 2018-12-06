#pragma once

#include "ofMain.h"
#include "ofxSerial.h"
#define USE_PI



#ifdef USE_PI
#include "ofxOMXPlayer.h"
#else

#endif


#define FRAMERATE 25
#define VIDEO_DISPLAY_SCALE 0.2

#define AUTO_RESTART 0


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
    void exit();
    
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );


    /*Serial */
    
    void serialSetup();
    
    vector<float> serialRead();
    vector<float> receivedVal;
    string receivedMsg;

    void sendChar(); 
    void sendDir(char a);
    void sendMoveTo(int b);
    
    vector<ofx::IO::BufferedSerialDevice> arduino; // 0 Accel, 1 disc, 2 screen
   // ofx::IO::BufferedSerialDevice deviceAccel;
      // ofx::IO::BufferedSerialDevice deviceDiscMotor;
   // ofx::IO::BufferedSerialDevice deviceScreenMotor;


    std::vector<SerialMessage> serialMessages;
    
    void onSerialBuffer(const ofx::IO::SerialBufferEventArgs& args);
    void onSerialError(const ofx::IO::SerialBufferErrorEventArgs& args);
    /* end of Serial */
    
    /*video */
    
    void videoPlayerSetup();
    void videoMixing();
        
#ifdef USE_PI
    ofxOMXPlayer vid1;
    ofxOMXPlayer vid2;
#else
    ofVideoPlayer vid1;
    ofVideoPlayer vid2;
#endif
    
    float vidDisplayScale;
    ofFbo finalVid;
        
        
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
    
    int mixingMode;
    int numOfMixingMode;
    int vid1YPos;
    int vid2YPos;
    bool changeYPos;
    bool changeVid;
    int vidChannel;
    
    
    /*end of video */
    
/* debug */
    bool debugMode;

/* end of debug */

    /* error checking */
    
        
    int serialFailCheck;
    /* end of error checking */ 


    void initReset();


};




