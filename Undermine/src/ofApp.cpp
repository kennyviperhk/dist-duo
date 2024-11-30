#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    debugMode = false;
    ofSetVerticalSync(true);
    
    initReset();
    
    //==================Init =========
    exhibitionMode = true;
    ofSetFrameRate(60);
    
    pauseTimeBetweenSpinLowest = 30000;
    pauseTimeBetweenSpinHighest = 50000;
    
    pauseTimeBetweenRestVidLowest = 3000;
    pauseTimeBetweenRestVidHighest = 5000;
    
    motorIntervalLowest = 1000;
    motorIntervalHighest = 10000;
    
    angleSpeedLowest = -43;
    angleSpeedHighest = -25;
    
    vid1FadeTime = 4.0;
    
    //==================Motor =========
    
    runMotor = false;
    isSpinning = false;
    
    motorStopSpeed = 140;
    motorSpinSpeed = 165;
    
    angleSpeed = ofRandom(angleSpeedLowest,angleSpeedHighest);
    
    pauseTimeBetweenSpin = ofRandom(pauseTimeBetweenSpinLowest,pauseTimeBetweenSpinHighest);
    nextSpinTime = pauseTimeBetweenSpin;
    //================== Video ==================    
    string vid1Name = "v3.mov";
    
  //  vidListSpin.push_back("v1.mov");
   // vidListSpin.push_back("v2.mov");
    vidListSpin.push_back("v3.mov");
vidListSpin.push_back("v4.mov");
    
    
    vidListRest.push_back("v5.mov");
    vidListRest.push_back("v6.mov");
    
    vidDone = false;
#ifdef USEPI
    string  videoPath = ofToDataPath(vidListRest[ofRandom(vidListRest.size())],true);
    ofxOMXPlayerSettings settings;
    settings.videoPath = videoPath;
    //settings.useHDMIForAudio = true;
    settings.enableTexture = true;
    settings.enableLooping = false;
    settings.enableAudio = false;
    vid1.setup(settings);
    
#else
    vid1.load(vidListRest[ofRandom(vidListRest.size())]);
    vid1.setLoopState(OF_LOOP_NONE);
    vid1.play();
    
#endif
    
    vid1.setVolume(0);
    
    //================== FBO ==================
    
    finalVid.allocate(ofGetWidth(), ofGetHeight());
    
    //init
    prevAngle = 0;
    prevAngleTime = 0;
    prevSwingTime = 0;
    hasToMove = false;

    centerPoint = 40;
    numOfMixingMode = 2;
    mixingMode = 1;    //mode 3 = dist.solo
    vidChannel = 1;
    
    ofHideCursor();
    
    //================= Smoothing ========
    numReadings = 10;
    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
        readings.push_back(0);
    }
}


void ofApp::initReset(){
    
    
    //================== Serial ==================
    
    std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
    
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
            
            if(portDesc.find("A50285") != std::string::npos || portDesc.find("852353330") != std::string::npos)
                
            {
                // Connect to the first matching device.
                
                bool success = deviceAccel.setup(devicesInfo[i], 115200);
                
                
                if(success)
                {
                    deviceAccel.unregisterAllEvents(this);
                    deviceAccel.registerAllEvents(this);
                    
                    ofLogNotice("ofApp::setup") << "Accel Successfully setup " << devicesInfo[i];
                    accelConnected = true;
                    
                }
                else
                {
                    ofLogNotice("ofApp::setup") << "Unable to setup " << devicesInfo[i];
                }
            }
            
            if(portDesc.find("A9EH") != std::string::npos || portDesc.find("A1024DEJ") != std::string::npos )
            {
                // Connect to the first matching device.
                
                bool success = deviceMotor.setup(devicesInfo[i], 115200);
                
                if(success)
                {
                    deviceMotor.unregisterAllEvents(this);
                    deviceMotor.registerAllEvents(this);
                    
                    ofLogNotice("ofApp::setup") << "Motor Successfully setup " << devicesInfo[i];
                    motorConnected = true;
                    
                }
                else
                {
                    ofLogNotice("ofApp::setup") << "Unable to setup " << devicesInfo[i];
                }
            }
        }
    }
    else
    {
        ofLogNotice("ofApp::setup") << "No devices connected.";
    }
    
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    //================== Serial ==================
    
    receivedVal = serialRead();
    currAngle = receivedVal[1];
    
    //================== Video ==================
#ifdef USEPI
    
#else
    vid1.update();
    
#endif
    currTime = ofGetElapsedTimeMillis();
    
    if( currTime - prevAngleTime >= 250){
        angleChangeSpeed = currAngle - prevAngle;
        prevAngle = currAngle;
        prevAngleTime = currTime;
    }
    
    if(exhibitionMode){
        if(isSpinning){
            runMotor = true;
        }else{
            runMotor = false;
        }

        

        
        if ( currTime >= nextMotorInterval ){
            motorInterval =  ofRandom(motorIntervalLowest, motorIntervalHighest);
            nextMotorInterval = currTime + motorInterval;
            if(motorInterval > motorIntervalHighest/2){
                motorSpinSpeed = ofRandom(155,170);// Long
            }else{
                motorSpinSpeed = ofRandom(160,180);// short
            }
            angleSpeed = ofRandom(angleSpeedLowest,angleSpeedHighest);
        }
        
        if(runMotor){
            if(angleChangeSpeed  > 0 ){
                sendMoveTo(motorStopSpeed);
                motorSpinSpeed = ofRandom(170,175);
            }
            else if(angleChangeSpeed  < angleSpeed ){
                sendMoveTo(motorStopSpeed);
                motorSpinSpeed = ofRandom(170,175);
            }
            else{
                sendMoveTo(motorSpinSpeed);
            }
        }
        else{
            sendMoveTo(motorStopSpeed);
        }
    }
    
     if(ofGetFrameNum() > 100 && ofGetFrameRate() <40){

     ofLog()<< "Too Slow, restart!";
         exhibitionMode = false;
         currMoveTo = 140;
        sendMoveTo(currMoveTo);
        ofLog() << "MoveTo sent : "<< currMoveTo << " Stop";

     std::exit(1);
     }
    
    //================ Smoothing ================= // Not used yet
    
    total = total - readings[readIndex];
    // read from the sensor:
    if(currAngle < 0){currAngle = currAngle- 360; }
    readings[readIndex] = currAngle;
    
    bool negReading = false;
    bool posReading = false;
    for(int i = 0; i < readIndex; i++){
        if (readings[readIndex] >= 0){
            negReading =true;
        }
        if (readings[readIndex] < 0){
            negReading = true;
        }
    }
    if (posReading & negReading){
        for(int i = 0; i < readIndex; i++){
            if (readings[readIndex] < 0){
                readings[readIndex]  + 360;
            }
        }
    }
    // add the reading to the total:
    total = total + readings[readIndex];
    // advance to the next position in the array:
    readIndex = readIndex + 1;
    
    // if we're at the end of the array...
    if (readIndex >= numReadings) {
        // ...wrap around to the beginning:
        readIndex = 0;
    }
    
    // calculate the average:
    //  sCurrAngle = total / numReadings;
    
    sCurrAngle = -currAngle;
}

void ofApp::videoMixing(){

    if(mixingMode ==0){
        
        ofBackground(255);
        ofSetColor(255);
        
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
        ofRotateZ(sCurrAngle);
        //ofRotateZ(currAngle+mouseX);
        ofTranslate(-ofGetWidth()/2,-ofGetHeight()/2);
        
        //for 720p video
        //vid1.draw(0, 0, finalVid.getWidth(), finalVid.getHeight());
        
        //for rect video
        vid1.draw((finalVid.getWidth()-finalVid.getHeight())/2, (finalVid.getHeight()-finalVid.getHeight())/2, finalVid.getHeight(), finalVid.getHeight());
        ofPopMatrix();
        
    }
    else if(mixingMode == 1){  //Current Mode
        
#ifdef USEPI
        
        float timeShiftForOMXBUG = 1.0; // fade out earlier due to getCurrentFrame() inaccuracy bug
        vid1ElapsedTime = vid1.getCurrentFrame()/vid1.getFPS();
        if(vid1ElapsedTime >vid1.getDurationInSeconds())
        {
            vid1ElapsedTime = vid1ElapsedTime - vid1.getDurationInSeconds();
        }
        vid1TimeRemaining = vid1.getDurationInSeconds() - timeShiftForOMXBUG - vid1ElapsedTime;
        
#else
        float timeShiftForOMXBUG = 1.0;
        vid1ElapsedTime = vid1.getPosition() * vid1.getDuration();
        vid1TimeRemaining = vid1.getDuration() - timeShiftForOMXBUG - vid1ElapsedTime;
        
#endif
        
        if(vid1ElapsedTime < vid1FadeTime){
            vid1Alpha = (vid1ElapsedTime/vid1FadeTime) * 255;
        }else if(vid1TimeRemaining< vid1FadeTime){
            if(vid1TimeRemaining < 0.8 && vidDone == false){
                vidDone = true;
            }
            
            if(vidDone){
                vid1Alpha--;
                if(vid1Alpha<=0){
                    vid1Alpha = 0;
                    if(isSpinning){
                        pauseTimeBetweenSpin = ofRandom(pauseTimeBetweenSpinLowest,pauseTimeBetweenSpinHighest);
                        nextSpinTime = currTime + pauseTimeBetweenSpin;
                        nextVidTime =   currTime + 30;
                        isSpinning = false;
                    }
                    pauseTimeBetweenRestVid = ofRandom(pauseTimeBetweenRestVidLowest,pauseTimeBetweenRestVidHighest);
                    nextVidTime =   currTime + pauseTimeBetweenRestVid;
                    
                    if(currTime >= nextSpinTime && !isSpinning){
                        isSpinning = true;
                        
                    }
                    if(isSpinning){
#ifdef USEPI
                        vid1.loadMovie(ofToDataPath(vidListSpin[ofRandom(vidListSpin.size())],true));
                        
#else
                        vid1.load(vidListSpin[ofRandom(vidListSpin.size())]);
#endif
                    
                    }
                    else{
#ifdef USEPI
                        vid1.loadMovie(ofToDataPath(vidListRest[ofRandom(vidListRest.size())],true));
                    
#else
                        vid1.load(vidListRest[ofRandom(vidListRest.size())]);
#endif
                    }
                    vid1.setPaused(true);
                
                }
            }else{
                vid1Alpha = ((vid1TimeRemaining / vid1FadeTime) *255);
            }
        }else{
            vid1Alpha = 255;
        }

        if(currTime >= nextVidTime){
            vidDone = false;
            
#ifdef USEPI
            vid1.setPaused(false);
#else
            vid1.play();
#endif
        }
        
        ofBackground(vid1Alpha);
        ofSetColor(vid1Alpha);
        
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
        if(isSpinning){
            ofRotateZ(sCurrAngle);
        }
        //ofRotateZ(currAngle+mouseX);
        ofTranslate(-ofGetWidth()/2,-ofGetHeight()/2);
        //for 720p video
        //vid1.draw(0, 0, finalVid.getWidth(), finalVid.getHeight());
        //for rect video
        vid1.draw((finalVid.getWidth()-finalVid.getHeight())/2, (finalVid.getHeight()-finalVid.getHeight())/2, finalVid.getHeight(), finalVid.getHeight());
        ofPopMatrix();
    }

}
//--------------------------------------------------------------
void ofApp::draw(){
    
    ofClear(0);
    ofBackground(0);
    
    finalVid.begin();
    videoMixing();
    finalVid.end();
    
    //====================== DEBUG MODE ====================
    
    if(debugMode){
        ofBackground(0);
        ofSetColor(255);
        
        std::stringstream ss;
        
        ss << "receivedMsg: " << receivedMsg << endl;
        ss << "FrameRate : "<< ofGetFrameRate() << endl;
        ss << "currAngle : "<< currAngle << endl;
        ss << "sCurrAngle : "<< sCurrAngle << endl;
        ss << "Angle Speed : "<< angleChangeSpeed << endl;
        ss << "Accel Connected : "<< accelConnected << endl;
        ss << "Motor Connected : "<< motorConnected << endl;
        ss << "exhibitionMode : "<< exhibitionMode << endl;
        ss << "vid1Alpha : "<< vid1Alpha << endl;
        ss << "vid1TimeRemaining : "<< vid1TimeRemaining << endl;
        ss << "vid1ElapsedTime : "<< vid1ElapsedTime << endl;
        ss << "currentTime : "<< currTime << endl;
        ss << "nextSpinTime(Count Down) : "<< currTime - nextSpinTime << endl;
        ss << "nextVidTime(Count Down) : "<< currTime - nextVidTime << endl;
        ss << "is Spinning : "<< isSpinning << endl;
        ss << "Motor Interval : "<< motorInterval << endl;
        ss << "Next Motor Interval (remaining) : "<< currTime - motorInterval << endl;
        ss << "MotorSpeed (currMoveTo) : "<< currMoveTo << endl;
        ss << "AngleSpeedLimit : "<< angleSpeed << endl;
        
        ofSetColor(255, 0, 0);
        ofDrawBitmapString(ss.str(), ofVec2f(20, 20));
        
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofTranslate(0,0);
        ofScale(0.5, 0.5);
        ofPushMatrix();
        if(receivedVal.size()>=3){
            //ofRotateX(receivedVal[0]);
            ofRotateZ(-sCurrAngle);
            //ofRotateZ(receivedVal[2]);
            
        }
        
        ofSetColor(255);
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
        ofRotateZ(sCurrAngle + mouseX);
        //ofRotateZ(currAngle+mouseX);
        ofTranslate(-ofGetWidth()/2,-ofGetHeight()/2);
        vid1.draw(0, 0, finalVid.getWidth(), finalVid.getHeight());
        ofPopMatrix();
        ofPopMatrix();
        ofPopMatrix();
        
    }else{
        
        ofSetRectMode(OF_RECTMODE_CORNER);
        finalVid.draw(0, 0, ofGetWidth(), ofGetHeight());
        
    }
    
}


void ofApp::sendMoveTo(int b){
    /*  vector<uint8_t> hi;
     hi.push_back(b);
     ofx::IO::ByteBuffer buffer(hi);
     deviceMotor.writeByte(hi[0]);
     */
    currMoveTo = b;
    deviceMotor.writeByte(b);
    
}



//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    currMoveTo = ofRandom(155, 170);
    switch(key){
        case 's':
            sendMoveTo(currMoveTo);
            ofLog() << "MoveTo sent : "<< currMoveTo;
            break;
        case 'a':
            currMoveTo = 140;
            sendMoveTo(currMoveTo);
            ofLog() << "MoveTo sent : "<< currMoveTo << " Stop";
            break;
        case 'd':
            debugMode = !debugMode;
            break;
        case 'r':
            initReset();
            break;
        case 'm':
            mixingMode++;
            if(mixingMode>= numOfMixingMode){
                mixingMode = 0;
            }
            break;
        case 'e':
            exhibitionMode = !exhibitionMode;
            nextSpinTime = currTime;
            break;
        case 't':
            std::exit(1);
            break;
            
    }
    
}





//--------------------SERIAL COMMUNICATION -------------------------------

void ofApp::sendChar(int a){
    vector<uint8_t> hi;
    hi.push_back(a);
    ofx::IO::ByteBuffer buffer(hi);
    deviceAccel.writeByte(hi[0]);
}


vector<float> ofApp::serialRead(){
    
    vector<float> currVal;
    std::vector<SerialMessage>::iterator iter = serialMessages.begin();
    
    receivedMsg = "";
    
    
    while (iter != serialMessages.end())
    {
        
        
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
    
    
    for(int i =0 ; i< receivedMsg.size(); i++){
        if(receivedMsg.find("euler") != std::string::npos){
            receivedMsg = receivedMsg.erase(0,3);
            //  ofLog() << "1. receivedMsg.erase(0,3) " << receivedMsg;
            receivedMsg = receivedMsg.erase(0,1);
            // ofLog() << "2. receivedMsg.erase(0,1) " << receivedMsg;
        }else{
            //sendChar(140);
        }
    }
    
    string s(receivedMsg);
    istringstream iss(s);
    
    do
    {
        string sub;
        iss >> sub;
        currVal.push_back(ofToFloat(sub));
        //cout << "Substring: " << sub << endl;
        
    } while (iss);
    
    
    return currVal;
    
}



void ofApp::onSerialBuffer(const ofx::IO::SerialBufferEventArgs& args)
{
    // Buffers will show up here when the marker character is found.
    SerialMessage message(args.getBuffer().toString(), "", 500);
    serialMessages.push_back(message);
}

void ofApp::onSerialError(const ofx::IO::SerialBufferErrorEventArgs& args)
{
    // Errors and their corresponding buffer (if any) will show up here.
    SerialMessage message(args.getBuffer().toString(),
                          args.getException().displayText(),
                          500);
    serialMessages.push_back(message);
}



//------------------------------------------------------------------

//------------UNUSED-----------

//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}




