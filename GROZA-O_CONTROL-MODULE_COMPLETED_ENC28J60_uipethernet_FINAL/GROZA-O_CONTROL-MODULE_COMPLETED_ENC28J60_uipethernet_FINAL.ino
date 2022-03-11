#include <UIPEthernet.h>
#include <SPI.h>
//enc28j60 module. USE PC NETWORK OPTIONS
static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE };
const static IPAddress localIP(192,168,5,25);
unsigned int localPort = 8995;    

//USRP
IPAddress remoteIP(192,168,5,3); 
unsigned int remotePort = 25000;

uint16_t package[25]; //data for transmission
EthernetUDP udp;
int op = 1;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial.setTimeout(10);
  //Serial.println("Initializing COM...");
  Ethernet.begin(mac,localIP);
  udp.begin(localPort);
  
  //print configuration parameters
  /*
  Serial.println("Network configuration:");
  Serial.println("-------------USRP-------------");
  Serial.print("remoteIP: ");
  Serial.println((char *)ipDestination);
  Serial.print("remotePort: ");
  Serial.println(remotePort);
  Serial.println("-------------W5500(PC)-------------");
  Serial.print("localIP: ");
  Serial.println((char *)ip);
  Serial.print("localPort: ");
  Serial.println(localPort);
  */
}

int hop = 0;

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
    
    Serial.println("\ndata:");   
     
    for(int i = 0; i < sizeof(package)/sizeof(package[0]); i++)
    {
      Serial.print(package[i]);
      Serial.print(" ");
    }
    Serial.println("");
//костыль только для enc и этой библиотеки
      if (op == 1)
      {
        op = 0;
        for (int i = 0; i < 15;i++)
        {
          udp.beginPacket(remoteIP,remotePort);
          udp.write((const char *)package, sizeof(package));
          udp.endPacket();
          delay(100);
        }
      }
      //
    udp.beginPacket(remoteIP,remotePort);
    udp.write((const char *)package, sizeof(package));
    udp.endPacket();
  }
}
