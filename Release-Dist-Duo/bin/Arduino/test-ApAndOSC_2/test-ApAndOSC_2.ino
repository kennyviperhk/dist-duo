#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoOSC.h>



#ifndef APSSID
#define APSSID "ESPap"
#define APPSK  "thereisnospoon"
#endif


/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;



// for ArduinoOSC
OscWiFi osc;
const char* host = "192.168.4.255";
const int recv_port = 8888;
const int send_port = 9001;

unsigned long previousMillis = 0;        // will store last time LED was updated

const long interval = 400;           // interval at which to blink (milliseconds)


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

}

void loop()
{
  //  osc.parse(); // should be called
 unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;

  int i = currentMillis;
  float f = 34.56F;
  String s = "hello";


  osc.send(host, send_port, "/send", i, f, s);
  }
}
