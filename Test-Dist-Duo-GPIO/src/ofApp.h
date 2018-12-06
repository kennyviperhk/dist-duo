#pragma once

#include "ofMain.h"
#include "ofxSerial.h"


#define USE_PI

#ifdef USE_PI
#include "ofxGPIO.h"
#include "ofxOMXPlayer.h"
#else

#endif

#define MPU 0x68

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


    	
		/* serial */
  /*  vector<float> serialRead();
    void sendChar();
    

    
    void onSerialBuffer(const ofx::IO::SerialBufferEventArgs& args);
    void onSerialError(const ofx::IO::SerialBufferErrorEventArgs& args);
    
    ofx::IO::BufferedSerialDevice deviceAccel;
    ofx::IO::BufferedSerialDevice deviceMotor;
    
    std::vector<SerialMessage> serialMessages;
    string receivedMsg;
    
    vector<float> receivedVal;
    * */
    /* end of serial */
		
		
		
#ifdef USE_PI
    /* mpu */
    I2c * bus;
    //float ap = 0.955;
    float ap = 0.1;
    double compAngleX, compAngleY,compAngleZ, timer;
    double accXangle , accYangle, accZangle;
    double gyroXrate , gyroYrate, gyroZrate;
    double gyroXAngle, gyroYAngle, gyroZAngle;

		
    void mpuSetup();
    void mpuUpdate();
    float mpuGetAngle();
    /* end of mpu */
		
		

#else

#endif

	/* Video */
    void videoSetup();
		    
    void videoMixing();
    	
#ifdef USE_PI
    ofxOMXPlayer vid1;
    ofxOMXPlayer vid2;
#else
    ofVideoPlayer vid1;
    ofVideoPlayer vid2;
#endif

    ofFbo finalVid;
    
    int vidChannel;
    float vidDisplayScale;

    /*end of video */
	
    
    float prevAngle;
    float currAngle;
    float angleChangeSpeed;
    
    int vid1YPos;
    int vid2YPos;
    bool changeYPos;
    
    
    float debugA;
    float debugB;
    	
    bool debugMode;
		
};
