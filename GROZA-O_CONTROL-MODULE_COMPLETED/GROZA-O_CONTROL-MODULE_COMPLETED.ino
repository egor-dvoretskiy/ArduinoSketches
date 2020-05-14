#include <SPI.h> 
#include <Ethernet2.h>

//W5500 module. USE PC NETWORK OPTIONS
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress localIP(192, 168, 5, 25);
unsigned int localPort = 8995;    

//USRP
IPAddress remoteIP(192, 168, 5, 3);     
unsigned int remotePort = 25000;

uint16_t package[25]; //data for transmission
EthernetUDP Udp;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial.setTimeout(10);
  //Serial.println("Initializing COM...");

  // set configuration for w5500:
  Ethernet.begin(mac,localIP);
  Udp.begin(localPort);
  //Serial.println("Setting local network configuration...");

  //print configuration parameters
  /*
  Serial.println("Network configuration:");
  Serial.println("-------------USRP-------------");
  Serial.print("remoteIP: ");
  Serial.println(remoteIP);
  Serial.print("remotePort: ");
  Serial.println(remotePort);
  Serial.println("-------------W5500(PC)-------------");
  Serial.print("localIP: ");
  Serial.println(localIP);
  Serial.print("localPort: ");
  Serial.println(localPort);
  */
}

void loop() {
  if (Serial.available() > 0) { 
    String recv = Serial.readString();      
    String str;
    int k = 0;
    for (int i = 0; i < recv.length();i++)
    {
      if (recv[i] != ' ' && recv[i] != '[' && recv[i] != ']')
      {
        str += recv[i];
        k++;
      }      
    }
    str += ",";
    
    String temp = "";
    int index = 0, h = 0;
    
    for (int i = 0; i < str.length(); i++)
    {
      if (str[i] == ',')
      {
        package[index] = temp.toInt();

        h = 0;
        temp = "";
        index++;
      }
      else
      {
        temp += str[i];
        h++;
      }
    }

    /*
       Serial.println("\nArray to send through UDP channel:");   
     
    for(int i = 0; i < sizeof(package)/sizeof(package[0]); i++)
    {
      Serial.print(package[i]);
      Serial.print(" ");
    }
    Serial.println("");
  
*/
    size_t sz = sizeof(package);
    
    Udp.beginPacket(remoteIP, remotePort);
    Udp.write((uint8_t *)package, sz);
    Udp.endPacket(); 
       
  }
}

