#pragma once

#define USEPI
//#define USESHADER


#include "ofMain.h"
#include "ofxSerial.h"

#ifdef USEPI
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
    
    
    void keyReleased(int key);
    
    
    
    vector<float> serialRead();
    void sendChar(int a);
    void sendMoveTo(int b);
    
    void videoMixing();
    
    void onSerialBuffer(const ofx::IO::SerialBufferEventArgs& args);
    void onSerialError(const ofx::IO::SerialBufferErrorEventArgs& args);
    
    ofx::IO::BufferedSerialDevice deviceAccel;
    bool accelConnected;
    ofx::IO::BufferedSerialDevice deviceMotor;
       bool motorConnected;
    
    std::vector<SerialMessage> serialMessages;
    string receivedMsg;
    
    vector<float> receivedVal;
    
    
    
#ifdef USEPI
    ofxOMXPlayer vid1;
#else
    ofVideoPlayer vid1;
#endif

    bool debugMode;
    
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
    void initReset();
    
    //=====Smoothing==========
    int numReadings;
    vector<float> readings;
    float sCurrAngle;
    float total;
    float average;
    int readIndex;
    
    //=====Video Mode==========
    
    int mixingMode;
    int numOfMixingMode;

    bool changeYPos;
    bool changeVid;
    int vidChannel;
    
    bool invertVid;
    
    int vid1Alpha;
    
    long pauseTimeBetweenSpin;
    int pauseTimeBetweenSpinLowest;
    int pauseTimeBetweenSpinHighest;
    
    long pauseTimeBetweenRestVid;
    int pauseTimeBetweenRestVidLowest;
    int pauseTimeBetweenRestVidHighest;

    //time for fading
    float vid1TimeRemaining;
    float vid1ElapsedTime;
    float vid1FadeTime;
    
    vector<string> vidListSpin;
    vector<string> vidListRest;
    bool vidDone;
    bool isSpinning;
    long nextVidTime;
    long nextSpinTime;
    bool runMotor;
    
    float angleSpeed;
    float angleSpeedLowest;
    float angleSpeedHighest;
    
    long motorInterval;
    int motorIntervalLowest;
    int motorIntervalHighest;
    long nextMotorInterval;
    
    int motorStopSpeed;
    int motorSpinSpeed;
    
    bool exhibitionMode;
    
    //=========unused=======
    
    void keyPressed(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
};




