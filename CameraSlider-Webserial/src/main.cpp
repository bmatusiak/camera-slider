#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

// Replace with your network credentials
const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

// Set web server port number to 80
AsyncWebServer server(80);

String inputString2;
bool stringComplete2 = false; 

String inputString;
bool stringComplete = false; 


void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received From WebSerial Data...");
  Serial.println("Received From WebSerial Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);//send data back to webserial
  Serial.println(d);//send data to serial
  Serial2.println(d);//send data to serial2
}

void setup() {
  
    Serial.begin(115200);
    Serial2.begin(115200);

    // Connect to Wi-Fi network with SSID and password
    Serial.print("Setting AP (Access Point)…");
    // Remove the password parameter, if you want the AP (Access Point) to be open
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    WebSerial.begin(&server);
    WebSerial.msgCallback(recvMsg);
    server.begin();
}


void loop() {
  while (Serial2.available()) {
    char out = (char)Serial2.read();
    if (out == '\n') {
      stringComplete = true;
    }else{
      inputString += out;
    }
    if (stringComplete) {
      WebSerial.println(inputString);
      Serial.println(inputString);
      inputString = "";
      stringComplete = false;
      // delay(100);
    }
  }
  while (Serial.available()) {
    char out = (char)Serial.read();
    if (out == '\n') {
      stringComplete2 = true;
    }else{
      inputString2 += out;
    }
    if (stringComplete2) {
      WebSerial.println(inputString2);//send data to webserial
      Serial2.println(inputString2);//send data to other serial
      inputString2 = "";
      stringComplete2 = false;
      // delay(100);
    }
  }
  return;
  
}
