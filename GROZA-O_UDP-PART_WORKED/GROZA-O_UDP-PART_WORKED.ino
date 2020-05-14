#include <SPI.h>        

#include <Ethernet2.h>


// Enter a MAC address and IP address for your controller below.

// The IP address will be dependent on your local network:

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress localIP(192, 168, 3, 1);
unsigned int localPort = 8995;      // local port to listen on

IPAddress remoteIP(192,168,3,2);
unsigned int remotePort = 25000;

uint16_t package[24];
uint16_t freq[3];
int counter = 0;

// An EthernetUDP instance to let us send and receive packets over UDP

EthernetUDP Udp;

void setup() {
package[0] = 960;
package[1] = 69;
package[2] = 50;
package[3] = 10220;
package[4] = 0;
package[5] = 4095;
package[6] = 0;
package[7] = 4095;
package[8] = 0;
package[9] = 4095;
package[10] = 0;
package[11] = 4095;
package[12] = 0;
package[13] = 4095;
package[14] = 0;
package[15] = 4095;
package[16] = 0;
package[17] = 4095;
package[18] = 0;
package[19] = 4095;
package[20] = 0;
package[21] = 4095;
package[22] = 0;
package[23] = 4095;

freq[0] = 960;
freq[1] = 2412;
freq[2] = 5785;

Serial.begin(115200);

  // start the Ethernet and UDP:
Ethernet.begin(mac,localIP);
Udp.begin(localPort);

Serial.println("Configuration:");
Serial.print("remoteIP: ");
Serial.println(remoteIP);
Serial.print("remotePort: ");
Serial.println(remotePort);
}



void loop() {
package[0] = freq[counter];
  
Serial.println("Package to send: ");
for(int i = 0; i < sizeof(package)/sizeof(package[0]); i++)
{
  Serial.print(package[i]);
  Serial.print(" ");
}
Serial.println("");
//Udp.sendPacket( (byte *) &package, remoteIP, remotePort);
size_t sz = sizeof(package);

Udp.beginPacket(remoteIP, remotePort);
Udp.write((uint8_t *)package, sz);
Udp.endPacket();

if (++counter > 2)
counter = 0;
delay(2000);
}

