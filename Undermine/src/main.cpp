#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    
    
    
    
    ofGLESWindowSettings settings;
    settings.width = 1280;
    settings.height = 960;
    settings.windowMode =OF_FULLSCREEN;
    settings.setGLESVersion(2);
    ofCreateWindow(settings);
    //ofSetupOpenGL(1920,1080,OF_FULLSCREEN);			// <-------- setup the GL context
    


	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
