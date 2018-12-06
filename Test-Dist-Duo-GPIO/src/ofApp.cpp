#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	debugMode = false;

#ifdef USE_PI
	mpuSetup();
#else

#endif

    videoSetup();
    
    
    
}


//--------------------------------------------------------------
void ofApp::update(){
#ifdef USE_PI
    mpuUpdate();
    float a = mpuGetAngle();
    float midPt = -130;
    currAngle = ofMap(a, midPt - 90,midPt + 90, -40,40);
    
#else

#endif


#ifdef USE_PI
    
#else
    vid1.update();
    vid2.update();
#endif

if(debugMode){
	ofShowCursor();
}else{
	ofHideCursor();
}

    finalVid.begin();
    ofClear(0);
    videoMixing();
    finalVid.end();
    

}


//--------------------------------------------------------------
void ofApp::draw(){
	

#ifdef USE_PI
	
#else

#endif
    ofSetColor(255);

    finalVid.draw(0,0,ofGetWidth(), ofGetHeight());
    
    if(debugMode){
    ofDrawBitmapStringHighlight("Angle X : " + ofToString(currAngle),20,20);
    ofDrawBitmapStringHighlight("Angle X Original: " + ofToString(mpuGetAngle()),20,40);
    ofDrawBitmapStringHighlight("Debug A: " + ofToString(debugA),20,60);

	}


}

//--------------------------------------------------------------
void ofApp::videoMixing(){
    int topAngle = 20;
    
    
	
  // float var = ofMap(mouseX, 0 ,ofGetWidth(), 0 ,10);
        float var = 4;
        ofBackground(0);
        ofSetColor(255);
        ofPushMatrix();
        
        ofTranslate(finalVid.getWidth()/2, finalVid.getHeight()/2);
        ofRotateZDeg(currAngle);
        ofTranslate(-finalVid.getWidth()/2, -finalVid.getHeight()/2);
        ofTranslate(finalVid.getWidth()/2*var, finalVid.getHeight()/2*var);
        
        //Modify Pos

        if(currAngle>=0){
            ofSetColor(255, ofMap(currAngle,0,topAngle,255,0));
        }else{
            ofSetColor(255, ofMap(currAngle,0,-topAngle,255,0));
        }
        float modX = ofMap(currAngle,-topAngle,topAngle,2500,-2500);
        //float modY =ofMap(abs(currAngle),0,90,0,200);
        
        if(vidChannel == 1){
            ofTranslate(modX, vid1YPos );
            vid1.draw(-finalVid.getWidth()*var, -finalVid.getHeight()*var, finalVid.getWidth()*(1+var), finalVid.getHeight()*(1+var));
        }
        else{
            ofTranslate(modX, vid2YPos );
            vid2.draw(-finalVid.getWidth()*var, -finalVid.getHeight()*var, finalVid.getWidth()*(1+var), finalVid.getHeight()*(1+var));
        }
        
        
        
        if(currAngle > topAngle || currAngle < -topAngle){
            changeYPos = true;
        }
        if(changeYPos){
            int chance =   (int)ofRandom(3);
            if(chance == 2){
                if(vid1YPos == 1200){
                    vid1YPos = -1200;
                    vid2YPos = -1200;
                }else{
                    vid1YPos = 1200;
                    vid2YPos = 1200;
                }
                
            }
            if(chance == 1){
                if(vidChannel == 1){
                    vidChannel = 2;
                }else{
                    vidChannel = 1;
                }
                
            }
            changeYPos = false;
        }
        
        ofPopMatrix();
	if(debugMode){
      //  ofLog() << "Scale : "<<var;
    }

}

#ifdef USE_PI
void ofApp::mpuSetup(){
	bus = new I2c("/dev/i2c-1");
	bus->addressSet(MPU);
	int8_t power = bus->readByte(MPU_POWER1);
	bus->writeByte(MPU_POWER1, ~(1 << 6) & power);	
}

void ofApp::mpuUpdate(){
int16_t temp = bus->readByte(MPU_TEMP1) << 8 | bus->readByte(MPU_TEMP2);
			int16_t xacc  = bus->readByte(MPU_ACCEL_XOUT1) << 8 | bus->readByte(MPU_ACCEL_XOUT2);
			int16_t yacc  = bus->readByte(MPU_ACCEL_YOUT1) << 8 | bus->readByte(MPU_ACCEL_YOUT2);
			int16_t zacc  = bus->readByte(MPU_ACCEL_ZOUT1) << 8 | bus->readByte(MPU_ACCEL_ZOUT2);
			int16_t xgyr  = bus->readByte(MPU_GYRO_XOUT1) << 8 | bus->readByte(MPU_GYRO_XOUT2);
			int16_t ygyr  = bus->readByte(MPU_GYRO_YOUT1) << 8 | bus->readByte(MPU_GYRO_YOUT2);
			int16_t zgyr  = bus->readByte(MPU_GYRO_ZOUT1) << 8 | bus->readByte(MPU_GYRO_ZOUT2);

			//temperature = ofToString((int)(temp/340.00+36.53));

			//Complementary filter
			
			debugA  = ofMap(mouseX, 0,ofGetWidth(), 0.1, 1.0);
			ap = debugA;
			accXangle = (atan2(yacc, zacc) * RAD_TO_DEG);
			accYangle = (atan2(xacc, zacc) * RAD_TO_DEG);
			accZangle = (atan2(xacc, yacc) * RAD_TO_DEG);
			gyroXrate = xgyr / 16.5;
			gyroYrate = ygyr / 16.5;
			gyroZrate = zgyr / 16.5;
			timer = ofGetElapsedTimeMillis();

			gyroXAngle += gyroXrate * (ofGetElapsedTimeMillis() - timer) / 1000;
			gyroYAngle += gyroYrate * (ofGetElapsedTimeMillis() - timer) / 1000;
			gyroZAngle += gyroZrate * (ofGetElapsedTimeMillis() - timer) / 1000;
			compAngleX = ap * (compAngleX + gyroXAngle) + (1 - ap) * accXangle;
			compAngleY = ap * (compAngleY + gyroYAngle) + (1 - ap) * accYangle;
			compAngleZ = ap * (compAngleZ + gyroZAngle) + (1 - ap) * accZangle;
}


float ofApp::mpuGetAngle(){
    return compAngleX;
    }
#else

#endif


void ofApp::videoSetup(){
    
    //OMX Setup
    
#ifdef USE_PI
    string  videoPath = ofToDataPath("face.mp4",true);
    ofxOMXPlayerSettings settings;
    settings.videoPath = videoPath;
    //settings.useHDMIForAudio = true;
    settings.enableTexture = true;
    settings.enableLooping = true;
    settings.enableAudio = false;
    vid1.setup(settings);
    
    string  videoPath2 = ofToDataPath("turn.mp4",true);
    ofxOMXPlayerSettings settings2;
    settings2.videoPath = videoPath2;
    //settings2.useHDMIForAudio = true;
    settings2.enableTexture = true;
    settings2.enableLooping = true;
    settings2.enableAudio = false;
    vid2.setup(settings2);
#else
    vid1.load("face.mp4");
    vid1.setLoopState(OF_LOOP_NORMAL);
    vid1.play();
    
    vid2.load("turn.mp4");
    vid2.setLoopState(OF_LOOP_NORMAL);
    vid2.play();
    
#endif
    vid1.setVolume(0);
    vid2.setVolume(0);
    
    
    
    //End of OMX Setup

    finalVid.allocate(ofGetWidth(), ofGetHeight());

    vidChannel = 1;
    vidDisplayScale = 0.2;
    
    vid1YPos = 1200;
    vid2YPos = 1000;
    

    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key){
	
	case 'd':
	debugMode = !debugMode;
	
	default:
	break;
    }
}
