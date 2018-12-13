#pragma once

#include "ofMain.h"
#include "ofxSerial.h"
//#define USE_PI



#ifdef USE_PI
#include "ofxOMXPlayer.h"
#else

#endif


#define FRAMERATE 30
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
    
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    
    
    /*Serial */
    
    void serialSetup();
    vector<float> serialUpdate();
    void serialDraw();
    
    vector<float> receivedVal;
    vector<float> accelVal;
    vector<float> discVal;
    vector<float> screenVal;
    
    ofxIO::BufferedSerialDevice arduinoA; // 0 Accel, 1 disc, 2 screen
    ofxIO::BufferedSerialDevice arduinoB; // 0 Accel, 1 disc, 2 screen
    ofxIO::BufferedSerialDevice arduinoC; // 0 Accel, 1 disc, 2 screen
    
    std::vector<SerialMessage> serialMessages;
    
    void sendChar(int a);
    void sendCommand(int ar, string s);
    
    void onSerialBuffer(const ofxIO::SerialBufferEventArgs& args);
    void onSerialError(const ofxIO::SerialBufferErrorEventArgs& args);
    

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
    
    bool isEye;
    void pushWheelToRight(int amount);
    void pushWheelToLeft(int amount);
    void changeSpeed(int amount);
    long swingMillis;
    long swingInterval;
    /*end of video */
    
    /* debug */
    bool debugMode;
    
    /* end of debug */
    
    /* error checking */
    
    
    int serialFailCheck;
    /* end of error checking */
    
    
    void initReset();
    
    
    
    bool isLeftEye;
    
    
    void testConnection();
    long testConnectionMillis;
    bool getRespondsFromDiscArduino;
    bool getRespondsFromScreenArduino;
    int testConnectionStage;
    

    
    
    
    /* Disc Motor*/
    int isDiscArduino;
    int isScreenArduino;
    int isAccelArduino;
    bool isClockWise;
    bool prevDirection;
    float getRPM;
    
    bool nextScreenTrigger;

    int topAngle;
    bool screenIsOnRight;
    bool screenIsOnLeft;
};





