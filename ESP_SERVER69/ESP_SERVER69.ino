/*
 *  This sketch demonstrates how to scan WiFi networks. 
 *  The API is almost the same as with the WiFi Shield library, 
 *  the most obvious difference being the different file you need to include:
 */
#include "ESP8266WiFi.h"

const char *essid = "OnePlus 5";
const char *key = "Braindead6233";

const int INNER_LED = 2;
const int timeCheck = 5000; // ms

void setup() {
  Serial.begin(115200);

  pinMode(INNER_LED,OUTPUT);
  digitalWrite(INNER_LED, HIGH);
  
  WiFi.hostname("esp_server69");
  WiFi.begin(essid,key);
  
  Serial.print("Connecting");  
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("Connected. Local network settings:");
  
  Serial.print("IP address: ");           
  Serial.println(WiFi.localIP());
  
  Serial.print("Subnet Mask: ");           
  Serial.println(WiFi.subnetMask());
  
  Serial.print("Gateway IP: ");           
  Serial.println(WiFi.gatewayIP());

  Serial.println("---------");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED)
  {      
    Serial.println("Connection +present");    
    digitalWrite(INNER_LED, LOW);
  }
  else
  {    
    Serial.println("Connection -lost");  
    WiFi.begin(essid,key);
    digitalWrite(INNER_LED, HIGH);
  }

  delay(timeCheck);  
}
