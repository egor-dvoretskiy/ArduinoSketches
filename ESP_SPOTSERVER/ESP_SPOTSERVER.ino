#include "ESP8266WiFi.h"
#include <WiFiUdp.h>

#define WIFI_SSID "esp-ap"
#define WIFI_PASSWORD "1234567890"
#define WIFI_CHANNEL 1
#define WIFI_ISHIDDEN false
#define WIFI_CONNECTIONS 2

#define LOCAL_PORT 25063

IPAddress WIFI_LOCALIP(192,168,1,69);
IPAddress WIFI_GATEWAY(192,168,1,254);
IPAddress WIFI_SUBNET(255,255,255,0);

WiFiUDP udp;
char incomingPacket[255];  // buffer for incoming packets

void setup() {
  Serial.begin(115200);
  Serial.println();
/*
  Serial.print("Setting IPConfiguration ... ");
  boolean res1 = WiFi.softAPConfig(WIFI_LOCALIP, WIFI_GATEWAY, WIFI_SUBNET);
  //if(WiFi.softAPIP() == WIFI_LOCALIP)
  if(res1 == true)
  {
    Serial.println("IPConfiguration is Ready");
               //  "Готово"
  }
  else
  {
    Serial.println("IPConfiguration is Failed!");
               //  "Настроить точку доступа не удалось"
  }
  */
  Serial.print("Setting soft-AP ... ");
  boolean res2 = WiFi.softAP(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL, WIFI_ISHIDDEN, WIFI_CONNECTIONS);
  if(res2 == true)
  {
    Serial.println("softAP is Ready");
               //  "Готово"
  }
  else
  {
    Serial.println("softAP creating is Failed!");
               //  "Настроить точку доступа не удалось"
  }

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());

  
  udp.begin(LOCAL_PORT);
  Serial.printf("Now listening at UDP port %d\n", LOCAL_PORT);
}

void loop() {

  Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
            //  "Количество подключенных станций = "
  delay(3000);

  int packetSize = udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, udp.remoteIP().toString().c_str(), udp.remotePort());
    int len = udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);

  }
}
