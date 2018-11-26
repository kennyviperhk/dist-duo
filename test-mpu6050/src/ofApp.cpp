
#include "ofApp.h"



 void ofApp::setup(){
			bus = new I2c("/dev/i2c-1");
			bus->addressSet(MPU);
			int8_t power = bus->readByte(MPU_POWER1);
			bus->writeByte(MPU_POWER1, ~(1 << 6) & power);
}

void ofApp::update(){
			int16_t temp = bus->readByte(MPU_TEMP1) << 8 | bus->readByte(MPU_TEMP2);
			int16_t xacc  = bus->readByte(MPU_ACCEL_XOUT1) << 8 | bus->readByte(MPU_ACCEL_XOUT2);
			int16_t yacc  = bus->readByte(MPU_ACCEL_YOUT1) << 8 | bus->readByte(MPU_ACCEL_YOUT2);
			int16_t zacc  = bus->readByte(MPU_ACCEL_ZOUT1) << 8 | bus->readByte(MPU_ACCEL_ZOUT2);
			int16_t xgyr  = bus->readByte(MPU_GYRO_XOUT1) << 8 | bus->readByte(MPU_GYRO_XOUT2);
			int16_t ygyr  = bus->readByte(MPU_GYRO_YOUT1) << 8 | bus->readByte(MPU_GYRO_YOUT2);
			int16_t zgyr  = bus->readByte(MPU_GYRO_ZOUT1) << 8 | bus->readByte(MPU_GYRO_ZOUT2);

			temperature = ofToString((int)(temp/340.00+36.53));

			//Complementary filter
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

void ofApp::draw(){
	ofBackgroundGradient(ofColor::black,ofColor::cyan);
	ofEnableDepthTest();
	cam.begin();
		ofRotateX(compAngleX);
		ofRotateY(compAngleY);
		ofRotateZ(compAngleZ);
		ofFill();
		ofSetColor(ofColor::red);
		ofDrawBox(500);
		ofNoFill();
		ofSetColor(ofColor::black);
		ofDrawBox(500);
	cam.end();
	ofDisableDepthTest();
	ofDrawBitmapStringHighlight("Temp:"+temperature+" C - FPS:"+ofToString(ofGetFrameRate()),20,20);
}

