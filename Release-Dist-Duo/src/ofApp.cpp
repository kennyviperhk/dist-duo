#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    isLeftEye = true;
    debugMode = false;
    
    ofSetFrameRate(FRAMERATE);
    
    initReset();
    serialSetup();
    videoPlayerSetup();
    
    
    
    ofHideCursor();
    
    //=====DEBUG =====
    serialFailCheck =0;
}

void ofApp::initReset(){
    //init
    prevAngle = 0;
    prevAngleTime = 0;
    prevSwingTime = 0;
    hasToMove = false;
    
    
    vid1YPos = 1200;
    vid2YPos = 1000;
    centerPoint = 40;
    numOfMixingMode = 3;
    mixingMode = 3;
    vidChannel = 1;
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //================== Serial ==================
    
    receivedVal = serialUpdate();
    if(receivedVal[0] == 0){
        accelVal = receivedVal;
        //  currAngle = accelVal[1];
        currAngle = accelVal[1];
    }else{
        discVal = receivedVal;
    }
    
    //================== Video ==================
#ifdef USE_PI
    
#else
    vid1.update();
    vid2.update();
#endif
    int awayFromCenterPt = 10;
    currTime = ofGetElapsedTimeMillis();
    if( currTime - prevSwingTime >= 7000 || hasToMove){
        if(leftSide){
            currMoveTo = ofRandom(5, centerPoint-awayFromCenterPt);
        }else{
            currMoveTo = ofRandom(centerPoint+awayFromCenterPt, 95);
        }
        leftSide = !leftSide;
        // currMoveTo = ofMap(mouseX,0,ofGetWidth(),0,95);
        hasToMove = false;
        //  sendMoveTo(currMoveTo);
        prevSwingTime = currTime;
    }
    if( currTime - prevAngleTime >= 500){
        angleChangeSpeed = currAngle - prevAngle;
        prevAngle = currAngle;
        prevAngleTime = currTime;
    }
    if(abs(angleChangeSpeed)  > 5){
        hasToMove = true;
        if(angleChangeSpeed >0){
            //goPos
        }else{
            //goNeg
        }
        
    }
    
    if(ofGetFrameNum() > 100 && AUTO_RESTART){
        if(ofGetFrameRate() <15){
            ofLog()<< "Too Slow, restart!";
            std::exit(1);
        }
        ofLog()<< "prevAngle" <<prevAngle;
        ofLog()<< "currAngle" <<currAngle;
        if(receivedVal[1]==0 || angleChangeSpeed == 0){
            
            serialFailCheck++;
            
        }else{
            serialFailCheck=0;
        }
        
        if(serialFailCheck > 100){
            ofLog()<< "No Serial, restart";
            std::exit(1);
        }
        
    }
    
    
    
    
}

void ofApp::videoMixing(){
    int topAngle = 20;
    if(mixingMode ==0){
        
        ofBackground(0);
        if(currAngle>=0){
            ofSetColor(255, ofMap(currAngle,0,topAngle,255,0));
        }else{
            ofSetColor(255, ofMap(currAngle,0,-topAngle,255,0));
        }
        // ofSetColor(255);
        
        vid1.draw(0, 0, finalVid.getWidth(), finalVid.getHeight());
    }else if(mixingMode ==1){
        
        ofBackground(0);
        ofSetColor(255);
        ofPushMatrix();
        ofTranslate(finalVid.getWidth()/2, 0);
        ofRotateZDeg(currAngle);
        ofTranslate(-finalVid.getWidth()/2, 0);
        vid2.draw(0, 0, finalVid.getWidth(), finalVid.getHeight());
        
        
        ofPopMatrix();
    }
    else{
        
        float var = 6;
        ofBackground(0);
        ofSetColor(255);
        ofPushMatrix();
        
        ofTranslate(finalVid.getWidth()/2, finalVid.getHeight()/2);
        ofRotateZDeg(currAngle);
        int valToOffset =0;
        //  valToOffset =  ofMap(mouseX, 0, ofGetWidth(), -finalVid.getWidth()*2, finalVid.getWidth()*2);
        ofTranslate(-finalVid.getWidth()/2 + valToOffset, -finalVid.getHeight()/2);
        ofTranslate(finalVid.getWidth()/2*var, finalVid.getHeight()/2*var);
        
        //Modify Pos
        
        if(currAngle>=0){
            ofSetColor(255, ofMap(currAngle,0,topAngle,255,0));
        }else{
            ofSetColor(255, ofMap(currAngle,0,-topAngle,255,0));
        }
        float modX;
        if(isLeftEye){
            modX = ofMap(currAngle,-topAngle,topAngle,0,-2500);
        }else{
            modX = ofMap(currAngle,-topAngle,topAngle,0,2500);
        }
        float modY =ofMap(abs(currAngle),0,90,0,500);
        
        
        if(vidChannel == 1){
            ofTranslate(modX, vid1YPos + modY);
            vid1.draw(-finalVid.getWidth()*var, -finalVid.getHeight()*var, finalVid.getWidth()*(1+var), finalVid.getHeight()*(1+var));
        }
        else{
            ofTranslate(modX, vid2YPos + modY);
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
        //ofLog() << "Scale : "<<var;
        
        
    }
}
//--------------------------------------------------------------
void ofApp::draw(){
    
    finalVid.begin();
    ofClear(0);
    videoMixing();
    finalVid.end();
    
    
    if(debugMode){
        ofBackground(0);
        ofSetColor(255);
        
        std::stringstream ss;
        
        
        ss << "FrameRate : "<< ofGetFrameRate() << endl;
        ss << "currAngle : "<< currAngle << endl;
        ss << "Angle Speed : "<< angleChangeSpeed << endl;
        ss << "Current Move To : "<< currMoveTo << endl;
        
        ofSetColor(255, 0, 0);
        ofDrawBitmapString(ss.str(), ofVec2f(20, 20));
        
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofTranslate(0, -ofGetHeight()/3);
        ofScale(0.7, 0.7);
        ofPushMatrix();
        if(receivedVal.size()>=3){
            //ofRotateX(receivedVal[0]);
            ofRotateZDeg(-currAngle);
            //ofRotateZDeg(receivedVal[2]);
            
        }
        ofTranslate(0, 800);
        
        
        ofSetColor(255, 0, 0);
        ofNoFill();
        ofSetLineWidth(2);
        //ofRect(0, 0, 210, 160);
        
        ofFill();
        ofSetColor(0, 0, 255);
        //ofRect(0, 0, 200, 150);
        ofSetColor(255);
        
        finalVid.draw(-finalVid.getWidth()* vidDisplayScale/2, -finalVid.getHeight() * vidDisplayScale/2, finalVid.getWidth()* vidDisplayScale, finalVid.getHeight() * vidDisplayScale);
        
        ofPopMatrix();
        ofPopMatrix();
        
        serialDraw();
        
        /*
         std::stringstream ss;
         
         ss << "         FPS: " << ofGetFrameRate() << endl;
         
         ss << "Connected to: " << arduinoA.port() << endl;
         ss << "Connected to: " << arduinoB.port() << endl;
         
         
         ofDrawBitmapString(ss.str(), ofVec2f(20, 20));
         
         int x = 20;
         int y = 50;
         int height = 20;
         
         auto iter = serialMessages.begin();
         
         // Cycle through each of our messages and delete those that have expired.
         while (iter != serialMessages.end())
         {
         iter->fade -= 50;
         
         if (iter->fade < 0)
         {
         iter = serialMessages.erase(iter);
         }
         else
         {
         ofSetColor(255, ofClamp(iter->fade, 0, 255));
         ofDrawBitmapString(iter->message, ofVec2f(x, y));
         
         y += height;
         
         if (!iter->exception.empty())
         {
         ofSetColor(255, 0, 0, ofClamp(iter->fade, 0, 255));
         ofDrawBitmapString(iter->exception, ofVec2f(x + height, y));
         y += height;
         }
         
         ++iter;
         }
         }
         */
        
    }else{
        
        ofSetRectMode(OF_RECTMODE_CORNER);
        finalVid.draw(0, 0, ofGetWidth(), ofGetHeight());
        
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key){
        case 'q':
            sendDir('l');
            break;
        case 'w':
            sendDir('r');
            break;
            
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    int a = ofRandom(5, 80);
    switch(key){
        case 's':
            sendChar(1);
            ofLog() << "char sent : ";
            break;
        case 'a':
            
            sendMoveTo(a);
            ofLog() << "MoveTo sent : "<< a;
            break;
        case 'd':
            debugMode = !debugMode;
            break;
        case 'r':
            initReset();
            break;
        case 'l':
            isLeftEye = !isLeftEye;
            break;
        case 'm':
            mixingMode++;
            if(mixingMode>= numOfMixingMode){
                mixingMode = 0;
            }
            
            break;
        case 't':
            std::exit(1);
            break;
            
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}



//===========================================
//================== Setup ==================
//===========================================


vector<float> ofApp::serialUpdate(){
    
    vector<float> currVal;
    
    
    
    std::vector<SerialMessage>::iterator iter = serialMessages.begin();
    
    string receivedMsg = "";
    
    
    while (iter != serialMessages.end())
    {
        
        iter->fade -= 50;
        
        if (iter->fade < 0)
        {
            iter = serialMessages.erase(iter);  //may need this to maintain performance
        }
        else
        {
            
            receivedMsg = iter->message;
            
            if (!iter->exception.empty())
            {
                // y += height;
            }
            
            ++iter;
        }
    }
    
    bool isAccelVal = false;
    for(int i =0 ; i< receivedMsg.size(); i++){
        if(receivedMsg.find("ypr") != std::string::npos){
            receivedMsg = receivedMsg.erase(0,3);
            receivedMsg = receivedMsg.erase(0,1);
            ofLog() << "ypr:" << receivedMsg;
            
            isAccelVal = true;
        }else if(receivedMsg.find("hz") != std::string::npos){
            receivedMsg = receivedMsg.erase(0,2);
            //  ofLog() << "1. hz(0,2) " << receivedMsg;
            receivedMsg = receivedMsg.erase(0,1);
            ofLog() << "Hz:" << receivedMsg;
            isAccelVal = false;
            
        }
    }
    
    string s(receivedMsg);
    istringstream iss(s);
    if(isAccelVal){
        currVal.push_back(0);
    }else{
        currVal.push_back(1);
    }
    do
    {
        string sub;
        iss >> sub;
        
        currVal.push_back(ofToFloat(sub));
        //cout << "Substring: " << sub << endl;
        
    } while (iss);
    
    
    return currVal;
    
    
}




void ofApp::sendChar(int a){
    
    vector<uint8_t> hi;
    hi.push_back(0);
    ofxIO::ByteBuffer buffer(hi);
    arduinoA.writeByte(hi[0]);
    arduinoB.writeByte(hi[0]);
    ofLog() << "send char ";
    
}

void ofApp::sendDir(int a){
    //  vector<uint8_t> hi;
    
    //     hi.push_back(a);
    //     hi.push_back('\n');
    
    
    //   std::string text = ofToString(a);
    //  ofx::IO::ByteBuffer textBuffer(text);
    
    // ofx::IO::ByteBuffer buffer(hi);
    // deviceMotor.writeByte(hi);
    
    
    //  for(int i=0;i<1000; i++){
    arduinoA.writeByte(a);
    arduinoB.writeByte(a);
    //  }
    
    /*for(int i=0;i<1000; i++){
     deviceDiscMotor.writeByte(a);
     //      deviceMotor.writeByte('\n');
     }*/
}


void ofApp::sendMoveTo(int b){
    /* vector<uint8_t> hi;
     hi.push_back(b);
     ofx::IO::ByteBuffer buffer(hi);
     deviceAccel.writeByte(hi[0]);*/
    
    arduinoA.writeByte(b);
    arduinoB.writeByte(b);
    
    
    
}


void ofApp::onSerialBuffer(const ofxIO::SerialBufferEventArgs& args)
{
    ofLog() << args.buffer().toString();
    
    // Buffers will show up here when the marker character is found.
    SerialMessage message(args.buffer().toString(), "", 500);
    serialMessages.push_back(message);
    
}

void ofApp::onSerialError(const ofxIO::SerialBufferErrorEventArgs& args)
{
    // Errors and their corresponding buffer (if any) will show up here.
    SerialMessage message(args.buffer().toString(),
                          args.exception().displayText(),
                          500);
    serialMessages.push_back(message);
    ofLog() << "serialMessages onSerialError : " << serialMessages.size();
}


//===========================================
//================== Setup ==================
//===========================================


void ofApp::serialSetup(){
    
    
    int a = 0;
    
    //================== Serial ==================
    
    vector<ofxIO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
    
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
            bool success  = false;
            if(a == 0){
                success = arduinoA.setup(devicesInfo[i], 115200);
            }else if (a == 1){
                success = arduinoB.setup(devicesInfo[i], 115200);
            }
            
            
            if(success)
            {
                if(a == 0){
                    arduinoA.unregisterAllEvents(this);
                    arduinoA.registerAllEvents(this);
                }else if (a == 1){
                    arduinoB.unregisterAllEvents(this);
                    arduinoB.registerAllEvents(this);
                }
                
                ofLogNotice("ofApp::setup") << "Successfully setup " << devicesInfo[i];
                
                a++;
                
            }
            else
            {
                ofLogNotice("ofApp::setup") << "Unable to setup " << devicesInfo[i];
            }
            
        }
    }
    else
    {
        ofLogNotice("ofApp::setup") << "No devices connected.";
    }
    
    
    ofLog() << "Serial Setup Done" ;
}





void ofApp::videoPlayerSetup(){
    
    //================== Video ==================
    
    vidDisplayScale = VIDEO_DISPLAY_SCALE;
    
    
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
    //vid1.loadMovie("face.mp4");
    vid1.setLoopState(OF_LOOP_NORMAL);
    vid1.play();
    
    vid2.load("turn.mp4");
    //vid2.loadMovie();
    vid2.setLoopState(OF_LOOP_NORMAL);
    vid2.play();
    
#endif
    
    vid1.setVolume(0);
    vid2.setVolume(0);
    //================== FBO ==================
    
    finalVid.allocate(ofGetWidth(), ofGetHeight());
    
    ofLog() << "Video Player Setup Done";
}




void ofApp::exit()
{
    
    
    arduinoA.unregisterAllEvents(this);
    arduinoB.unregisterAllEvents(this);
    
    
}




void ofApp::serialDraw(){
    
    ofSetColor(255);
    
    std::stringstream ss;
    
    std::stringstream ss2;
    
    ss << "         FPS: " << ofGetFrameRate() << endl;
    
    // for(int i=0; i< arduino.size(); i++){
    ss << "Connected to: " << arduinoA.port()<< endl;
    ss << "Connected to: " << arduinoB.port()<< endl;
    
    for(int i = 0; i< accelVal.size(); i++){
        ss << "accelVal from Arduino: " << i << " : "<< accelVal[i] << endl;
    }
    
    for(int i = 0; i< discVal.size(); i++){
        ss2 << "discVal from Arduino: " << i << " : "<< discVal[i] << endl;
    }
    
    //  }
    
    ofDrawBitmapString(ss.str(), ofVec2f(20, 200));
    
    ofDrawBitmapString(ss2.str(), ofVec2f(20, 400));
    
    
}

