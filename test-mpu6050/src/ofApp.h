#pragma once

#include "ofMain.h"
#include "ofxGPIO.h"


#define MPU 0x68

class ofApp : public ofBaseApp{
    public:
        void setup();
        void update();
        void draw();

		I2c * bus;
		float ap = 0.955;
		double compAngleX, compAngleY,compAngleZ, timer;
		double accXangle , accYangle, accZangle;
		double gyroXrate , gyroYrate, gyroZrate;
		double gyroXAngle, gyroYAngle, gyroZAngle;
		string temperature;
		ofEasyCam cam;
		
    
};
