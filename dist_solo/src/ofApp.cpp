#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    debugMode = false;
    ofSetFrameRate(25);
    
    initReset();
    //================== Video ==================
    
    
    vidDisplayScale = 0.2;
    
    
    
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
    
    sendChar();
    
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
    

    
    ofHideCursor();
    
    //=====DEBUG =====
    serialFailCheck =0;
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
            string portDesc = devicesInfo[i].getDescription();
            ofLog() << "devicesInfo[i].getDescription() : " << devicesInfo[i].getDescription();
            
            if(portDesc.find("CP2102") != std::string::npos )
           //                 if(portDesc.find("FTDI") != std::string::npos )
            {
                // Connect to the first matching device.
                
                bool success = deviceAccel.setup(devicesInfo[i], 115200);
                
                if(success)
                {
                    deviceAccel.unregisterAllEvents(this);
                    deviceAccel.registerAllEvents(this);
                    
                    ofLogNotice("ofApp::setup") << "Accel Successfully setup " << devicesInfo[i];
                    
                }
                else
                {
                    ofLogNotice("ofApp::setup") << "Unable to setup " << devicesInfo[i];
                }
            }
            else if(portDesc.find("USB2.0-Serial") != std::string::npos )
            {
                // Connect to the first matching device.
                bool success = deviceMotor.setup(devicesInfo[i], 115200);
                
                if(success)
                {
                    deviceMotor.registerAllEvents(this);
                    
                    ofLogNotice("ofApp::setup") << "Motor Successfully setup " << devicesInfo[i];
                    
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
        sendMoveTo(currMoveTo);
        prevSwingTime = currTime;
    }
    if( currTime - prevAngleTime >= 500){
        angleChangeSpeed = currAngle - prevAngle;
        prevAngle = currAngle;
    }
    if(abs(angleChangeSpeed)  > 5){
        hasToMove = true;
        if(angleChangeSpeed >0){
            //goPos
        }else{
            //goNeg
        }
        
    }
    
    if(ofGetFrameNum() > 100 ){
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
        ofRotateZ(currAngle);
        ofTranslate(-finalVid.getWidth()/2, 0);
        vid2.draw(0, 0, finalVid.getWidth(), finalVid.getHeight());
        
        
        ofPopMatrix();
    }
    else{
        // float var = ofMap(mouseX, 0 ,ofGetWidth(), 0 ,10);
        float var = 4;
        ofBackground(0);
        ofSetColor(255);
        ofPushMatrix();
        
        ofTranslate(finalVid.getWidth()/2, finalVid.getHeight()/2);
        ofRotateZ(currAngle);
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
        ofLog() << "Scale : "<<var;
        
        
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
        
        ss << "receivedMsg: " << receivedMsg << endl;
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
            ofRotateZ(-currAngle);
            //ofRotateZ(receivedVal[2]);
            
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
            sendChar();
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

//--------------------------------------------------------------

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
        if(receivedMsg.find("ypr") != std::string::npos){
            receivedMsg = receivedMsg.erase(0,3);
            //  ofLog() << "1. receivedMsg.erase(0,3) " << receivedMsg;
            receivedMsg = receivedMsg.erase(0,1);
            // ofLog() << "2. receivedMsg.erase(0,1) " << receivedMsg;
        }else{
            sendChar();
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


void ofApp::sendChar(){
    vector<uint8_t> hi;
    hi.push_back(0);
    ofx::IO::ByteBuffer buffer(hi);
    deviceAccel.writeByte(hi[0]);
}

void ofApp::sendDir(char a){
    //  vector<uint8_t> hi;
    
    //     hi.push_back(a);
    //     hi.push_back('\n');
    
    
    //   std::string text = ofToString(a);
    
    //  ofx::IO::ByteBuffer textBuffer(text);
    
    // ofx::IO::ByteBuffer buffer(hi);
    // deviceMotor.writeByte(hi);
    for(int i=0;i<1000; i++){
        deviceMotor.writeByte(a);
        //      deviceMotor.writeByte('\n');
    }
}


void ofApp::sendMoveTo(int b){
    /* vector<uint8_t> hi;
     hi.push_back(b);
     ofx::IO::ByteBuffer buffer(hi);
     deviceAccel.writeByte(hi[0]);*/
    deviceMotor.writeByte(b);
    
}



