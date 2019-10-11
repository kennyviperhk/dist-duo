
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoOSC.h>

//Config
#ifndef APSSID
#define APSSID "dist.duo"
#define APPSK  "helloworld"
#endif
const char* host = "192.168.4.255"; //broadcast
const int recv_port = 8888;
const int send_port = 9001;


// for ArduinoOSC
OscWiFi osc;
/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;


unsigned long previousMillis = 0;        // will store last time LED was updated

const long interval = 400;           // interval at which to blink (milliseconds)




/*
   PIR
*/
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 10;


//the time when the sensor outputs a low impulse
long unsigned int lowIn;

//the amount of milliseconds the sensor has to be low
//before we assume all motion has stopped
long unsigned int pause = 5000;

boolean lockLow = true;
boolean takeLowTime;

int pirPin = 13;    //the digital pin connected to the PIR sensor's output
int ledPin = LED_BUILTIN;

int motionState = LOW;
bool resetOnce = true;
bool isHuman = false;
int stage = -1;

//
int relayPin = 11;
int relayPin2 = 12;
int relayState = LOW;             // ledState used to set the LED
#include <Metro.h> // Include Metro library
long intervalTime = 900000; // 20 min
//long intervalTime = 5000;
Metro metroRelay = Metro(intervalTime);



/*
   PIR ends
*/

void setup()
{
  Serial.begin(115200);

  /*
      // WiFi stuff
      WiFi.begin(ssid, pwd);
      WiFi.config(ip, gateway, subnet);
      while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(500); }
      Serial.print("WiFi connected, IP = "); Serial.println(WiFi.localIP());
  */
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // ArduinoOSC
  osc.begin(recv_port);


  /*

     PIR

  */

  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);
  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  delay(50);

}

void loop()
{
  //  osc.parse(); // should be called
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;
    String s;
    if (relayState == HIGH) {
      s = "on";
    } else {
      s = "off";
    }

    osc.send(host, send_port, "/pir", s);
  }

  if (digitalRead(pirPin) == HIGH) {
    digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
    if (lockLow) {
      //makes sure we wait for a transition to LOW before any further output is made:
      lockLow = false;
      Serial.println("---");
      Serial.print("motion detected at ");
      Serial.print(millis() / 1000);
      Serial.println(" sec");
      motionState = true;
      delay(50);
    }
    takeLowTime = true;
  }

  if (digitalRead(pirPin) == LOW) {
    digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state

    if (takeLowTime) {
      lowIn = millis();          //save the time of the transition from high to LOW
      takeLowTime = false;       //make sure this is only done at the start of a LOW phase
    }
    //if the sensor is low for more than the given pause,
    //we assume that no more motion is going to happen
    if (!lockLow && millis() - lowIn > pause) {
      //makes sure this block of code is only executed again after
      //a new motion sequence has been detected
      lockLow = true;
      Serial.print("motion ended at ");      //output
      Serial.print((millis() - pause) / 1000);
      Serial.println(" sec");
      motionState = false;
      delay(50);
    }
  }


  if (motionState) {
    isHuman = true;
    metroRelay.reset();
    metroRelay.interval(intervalTime);
    relayState = HIGH;
    digitalWrite(relayPin, relayState);
    digitalWrite(relayPin2, relayState);
    Serial.println("---");
    Serial.print("power on at ");
    Serial.print(millis() / 1000);
    Serial.println(" sec");
  } else {

  }


  if (metroRelay.check() == 1 && isHuman) {
    Serial.println("---");
    Serial.print("power off at ");
    Serial.print(millis() / 1000);
    Serial.println(" sec");
    relayState = LOW;
    digitalWrite(relayPin, relayState);
    digitalWrite(relayPin2, relayState);
    isHuman = false;
  }
}
