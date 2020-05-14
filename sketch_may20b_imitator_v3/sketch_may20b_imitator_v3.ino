/////****** Imitator v.3 (Одинаковые серийники, 12 дронов, трейс в виде пентаграммы)*****////////
/////////////////////////////////////////////////////////////////////////////////////////////////
#include <ESP8266WiFi.h>

#define LED_INNER 2

extern "C" {
  #include "user_interface.h"
}

class Drones
{
  public:
    byte wifipkt[270] = {
      0x80,                      // type/subtype
      0x00,                      // flags
      0x00, 0x00,                // duration
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  // dst
      0x60, 0x60, 0x1F, 0x23, 0x24, 0x25,  // src
      0x60, 0x60, 0x1F, 0x23, 0x24, 0x25,  // bssid
      0x01, 0x02,                         // seq
      0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,      // timestamp value
      0x64, 0x00,                  // beacon interval
      0x00, 0x05,                  // capability flags
      // ssid length and ssid 
      0x00,                                                                   //header
      0x0c,                                                                   //ssid length
      0x53, 0x69, 0x6d, 0x75, 0x6c, 0x61, 0x74, 0x6f, 0x72, 0x20, 0x44, 0x4e, //ssid
      // supported rates
      0x01, 0x08, 0x82, 0x84, 0x8b, 0x0c, 0x12, 0x96, 0x18, 0x24, 
      // current channel
      0x03, 0x01, 0x0b, 
      // traffic indication map
      0x05, 0x04, 0x00, 0x01, 0x00, 0x00, 
      // country information
      0x07, 0x06, 0x55, 0x53, 0x00, 0x01, 0x0b, 0x1e, 
      // erp information
      0x2a, 0x01, 0x00, 
      // extended supported rates
      0x32, 0x04, 0x30, 0x48, 0x60, 0x6c, 
      // HT Capabilities
      0x2d, 0x1a, 0xac, 0x01, 0x02, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      // HT Information
      0x3d, 0x16, 0x0b, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      // RSN(Robust Secure Network) Information
      0x30, 0x14, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x02, 0x0c, 0x00, 
      // Vendor Specific: Microsoft WMM/WME Paramater Element
      0xdd, 0x18, 0x00, 0x50, 0xf2, 0x02, 0x01, 0x01, 0x00, 0x00, 0x03, 0xa4, 0x00, 0x00, 0x27, 0xa4, 0x00, 0x00, 0x42, 0x43, 0x5e, 0x00, 0x62, 0x32, 0x2f, 0x00,
      // fixedDroneID sample - https://github.com/DJISDKUser/metasploit-framework/blob/62e36f1b5c6cae0abed9c86c769bd1656931061c/modules/auxiliary/dos/wifi/droneid.rb#L93
      //
      0xdd, 0x52, 0x26, 0x37, 0x12, 0x58, 0x62, 0x13,              //header           do not change
      0x10,                                                        //sub_command      do not change 0x10
      0x01,                                                        //version          1
      0x5a, 0x00,                                                  //seq              90
      0xd7, 0x0f,                                                  //state_info       55055  
      0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35,  //SerialNum i = (200:215)
      0xea, 0xca, 0x49, 0x00, //27                                 //RawLon i = (216:219)
      0x7e, 0x95, 0x8f, 0x00, //54                                 //RawLat  i = (220:223)
      0x70, 0x00,         //112                                    //Altitude
      0x21, 0x00,         //33                                     //Height
      0xc9, 0x00,         //201                                    //V_north
      0xc7, 0x00,         //199                                    //V_east
      0x45, 0x00,         //69                                     //V_up
      0xc0, 0x00,                                                  //Pitch
      0xf4, 0x00,                                                  //Roll
      0x40, 0x00,                                                  //Yaw                                                                                                     
      0x6a, 0x73, 0x0c, 0x00,  //39.971056                         //RawHomeLon                                                                                                    
      0x95, 0x26, 0x75, 0x00,  //56.004938                         //RawHomeLat
      0x03,                                                        //ProductType i = 248
      0x06,                                                        //UUID length
      0x31, 0x39, 0x35, 0x37, 0x34, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //UUID 
      }; 
};
  
Drones* drone1 = new Drones(); Drones* drone2 = new Drones(); Drones* drone3 = new Drones();
Drones* drone4 = new Drones(); Drones* drone5 = new Drones(); Drones* drone6 = new Drones();
Drones* drone7 = new Drones(); Drones* drone8 = new Drones(); Drones* drone9 = new Drones(); 
Drones* drone10 = new Drones(); Drones* drone11 = new Drones(); Drones* drone12 = new Drones();

Drones* packDrones[] = {drone1, drone2, drone3, drone4, drone5, drone6,
                        drone7, drone8, drone9, drone10, drone11, drone12 };

byte ASCIIhex[] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 
                    0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 
                    0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 
                    0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b };
                    
byte productType[] = {  0x0b,     //Inspire 2
                        0x0c,     //Phantom 4
                        0x29,     //Mavic 2
                        0x1c,     //M210
                        0x0f,
                        0x01,
                        0x05,
                        0x0a,
                        0x0b,
                        0x12,
                        0x13,
                        0x24,
                        0x2c,
                        0x28,
                        0x17  };

byte traceLatitude[] = {  0x1b, 0xc5, 0x49, 0x00,
                          0xe9, 0xc9, 0x49, 0x00,
                          0x2c, 0xcf, 0x49, 0x00,
                          0x90, 0xc7, 0x49, 0x00,
                          0x4e, 0xbf, 0x49, 0x00,
                          0x0c, 0xc5, 0x49, 0x00,
                          0x55, 0xcc, 0x49, 0x00,
                          0x9f, 0xc9, 0x49, 0x00,
                          0x36, 0xc7, 0x49, 0x00,
                          0x56, 0xc5, 0x49, 0x00}; //49

byte traceLongitude[] = { 0x1a, 0x8d, 0x8f, 0x00,
                          0xef, 0x8f, 0x8f, 0x00,
                          0xcd, 0x92, 0x8f, 0x00,
                          0xcd, 0x92, 0x8f, 0x00,
                          0x7e, 0x92, 0x8f, 0x00,
                          0xf8, 0x8f, 0x8f, 0x00,
                          0xdc, 0x8b, 0x8f, 0x00,
                          0x97, 0x90, 0x8f, 0x00,
                          0xaa, 0x95, 0x8f, 0x00,
                          0xa1, 0x91, 0x8f, 0x00}; //8f

int mc = 0;

void setup() {
  delay(500);
  pinMode(LED_INNER,OUTPUT);
  digitalWrite(LED_INNER,LOW);
  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(1); 
  Serial.begin(115200);   
  for (int j = 0; j < sizeof(packDrones)/sizeof(packDrones[0]); j++)
  {
    for (int i = 200; i <= 215; i++)
    {
      packDrones[j]->wifipkt[i] = ASCIIhex[random(36)];     
    }
    packDrones[j]->wifipkt[248] = productType[random(sizeof(productType)/sizeof(productType[0]))];
  }
  
  //for (int j = 0; j < sizeof(packDrones)/sizeof(packDrones[0]); j++)
  //{
    //Serial.print("drone");
    //Serial.print(j+1);    
    //Serial.print(": ");    
    //for (int i = 200; i <= 215; i++)
    //{
    //  Serial.print(packDrones[j]->wifipkt[i]);     
    //}
    //Serial.print("\tType: ");
    //Serial.print(packDrones[j]->wifipkt[248]);
    //Serial.println("");
  //} 
}

void loop() {   
  for (int j = 0; j < sizeof(packDrones)/sizeof(packDrones[0]); j++)
  {
    for (int i = 216, k = 0; i <= 223; i++, k++)
    {      
        if (i >= 216 && i <= 219)
          packDrones[j]->wifipkt[i] =  traceLatitude[k+4*mc];
        else if (i >= 220 && i <= 223)  
        {
          k-=4;
          packDrones[j]->wifipkt[i] = traceLongitude[k+4*mc]; 
          k+=4;
        }
    } 
    wifi_send_pkt_freedom(packDrones[j]->wifipkt, 269, true);  
    delay(20); 
  }
  
  mc++;
  if (mc > 9) mc = 0;
}
