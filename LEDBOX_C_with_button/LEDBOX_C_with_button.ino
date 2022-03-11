#include "ESP8266WiFi.h"
#include <WiFiUdp.h>
#include <Bounce2.h>

#define RED 5   // d1
#define GREEN 4 // d2
#define BLUE 0  // d3
#define BUTTON 2 // d5

#define PARAMETERS_AMOUNT 6

#define pressed_long 2000 // долговременное нажатие = 2 секунды
Bounce bouncer = Bounce(); //создаем экземпляр класса Bounce
unsigned long pressed_moment; // момент нажатия кнопки

//const char *essid = "Veronika";
//const char *key = "1234567890";

const char *essid = "HUAWEI-WeGd";
const char *key = "48575443EDE2239D";

int amountOfSpots = 2;

String spots_essid[] =
{
   "HUAWEI-WeGd",
   "Veronika"
};

String spots_key[] = 
{
  "48575443EDE2239D",
  "1234567890"
};

const IPAddress staticIP(192, 168, 100, 73); //ESP static ip
const IPAddress gateway(192, 168, 100, 1);   //IP Address of your WiFi Router (Gateway)
const IPAddress subnet(255, 255, 255, 0);  //Subnet mask
const IPAddress dns(8, 8, 8, 8);  //DNS
const int localUdpPort = 25073;

WiFiUDP udp;
char incomingPacket[255];
const int connectionLimitTimes = 10;

const int max_value = 255;
const int min_value = 0;
int step_value = 1;
int speed_step_value = 20;
int delay_assign_mode = 5;
int threshold = 100;

int BRIGHTNESS = 199;
int currentMode = 15;
int currentValue = min_value;
int currentWay_heartbeat = 1;
int currentWay_anxiety = 1;
int currentWay_snow = 1;
bool upwardMovement = true;

const int b_setup_value_ms = 2000;
const int s_setup_value_ms = 500;
const int inputDelayWIFIConnection = 500;

void setup() {
  Serial.begin(115200);
  
  pinMode(BUTTON ,INPUT); // кнопка на пине 2
  digitalWrite(BUTTON ,HIGH); // подключаем встроенный подтягивающий резистор
  bouncer.attach(BUTTON); // устанавливаем кнопку
  bouncer.interval(5); // устанавливаем параметр stable interval = 5 мс
  
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);

  //WIFIConnection();
  ConnectToWIFISpot();
}

void loop() {
  if (bouncer.update())
  { //если произошло событие
    if (bouncer.read()==0)
    { //если кнопка нажата
      pressed_moment = millis(); // запоминаем время нажатия
    }
    else
    { // кнопка отпущена
      if((millis() - pressed_moment) < pressed_long)
      { // если кнопка была нажата кратковременно
        IncreaseCurrentModeValue(); // увеличиваем счетчик текушего режима
      }
      else
      { // кнопка удерживалась долго
        //WIFIConnection();
        ConnectToWIFISpot();
        currentMode = 15; 
        pressed_moment = 0; // обнуляем момент нажатия
      }
      f_reset();
      
      Serial.print("Current mode: ");
      Serial.println(currentMode); // выводим сообщение о текущем режиме
    }
  }
 
  int packetSize = udp.parsePacket();
  if (packetSize)
  {
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, udp.remoteIP().toString().c_str(), udp.remotePort());
           
    int len = udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    /*
    String s = String(incomingPacket);
    int pos = s.indexOf(" ");
    String s1 = s.substring(0, pos);
    String s2 = s.substring(pos + 1, len);

    currentMode = s1.toInt();
    BRIGHTNESS = s2.toInt();
    
    Serial.printf("currentMode: %d\n", currentMode);
    Serial.printf("currentBrightness: %d\n", BRIGHTNESS);
    */

    ParseIncomingPackage(incomingPacket, len);
    PrintParameters();
    
    f_reset();    
    
  }

  //Serial.printf("currentValue = %i\n", currentValue);
  useSpecificMode();
  
}

void IncreaseCurrentModeValue()
{
  currentMode = ++currentMode > 15 ? 0 : currentMode;
}

void SettingsPreset(int bright = 199, int step1 = 1, int step2 = 20, int adelay = 5, int thres = 100)
{
  BRIGHTNESS = bright;
  step_value = step1;
  speed_step_value = step2;
  delay_assign_mode = adelay;
  threshold = thres;
}

void SetDefaultSettingsForMode(int currentMode)
{
  // brightness, step_value, speed_step_value, delay_assign_mode, threshold
  switch(currentMode)
    {
      case 0:
        SettingsPreset(199, 1, 20, 5, 100);
        break;
      case 1:
        SettingsPreset(199, 3, 3, 5, 126);
        break;
      case 2:
        SettingsPreset(199, 1, 20, 5, 100);
        break;
      case 3:
        SettingsPreset(199, 3, 3, 5, 126);
        break;
      case 4:
        SettingsPreset(199, 1, 20, 5, 100);
        break;
      case 5:
        SettingsPreset(199, 3, 3, 5, 126);
        break;
      case 6:
        SettingsPreset(199, 3, 3, 5, 100);
        break;
      case 7:
        SettingsPreset(199, 3, 3, 5, 100);
        break;
      case 8:
        SettingsPreset(199, 3, 3, 5, 100);
        break;
      case 9:
        SettingsPreset(199, 3, 3, 5, 100);
        break;
      case 10:
        SettingsPreset(199, 2, 14, 10, 99);
        break;
      case 11:
        SettingsPreset(199, 1, 5, 20, 73);
        break;
      case 12:
        SettingsPreset(199, 1, 1, 11, 71);
        break;
      case 13:
        SettingsPreset(199, 1, 1, 30, 126);
        break;
      case 14:
        SettingsPreset(199, 5, 5, 101, 126);
        break;
      case 15:
        SettingsPreset(199, 1, 20, 5, 100);
        break;
      default:
        Serial.println("Wrong mode");
        break;
    }
}

void ParseIncomingPackage(char* ipckg, int len)
{
  String s = String(incomingPacket);

  for(int i = 0; i < PARAMETERS_AMOUNT; i++)
  {
    int pos = s.indexOf(" ");
    String s1 = s.substring(0, pos);
    s = s.substring(pos + 1, len);
    /*
    Serial.printf("-\npos: %d\n", pos);
    Serial.printf("num: %d\n", i);
    Serial.println("s: " + s);
    Serial.println("s1: " + s1);
    Serial.println("-");
    */
    switch(i)
    {
      case 0:
        currentMode = s1.toInt();
        break;
      case 1:
        BRIGHTNESS = s1.toInt();
        break;
      case 2:
        step_value = s1.toInt();
        break;
      case 3:
        speed_step_value = s1.toInt();
        break;
      case 4:
        delay_assign_mode = s1.toInt();
        break;
      case 5:
        threshold = s1.toInt();
        break;
    }
  }  
}

void PrintParameters()
{
  Serial.printf("currentMode: %d\n", currentMode);
  Serial.printf("BRIGHTNESS: %d\n", BRIGHTNESS);
  Serial.printf("step_value: %d\n", step_value);
  Serial.printf("speed_step_value: %d\n", speed_step_value);
  Serial.printf("delay_assign_mode: %d\n", delay_assign_mode);
  Serial.printf("threshold: %d\n", threshold);
  Serial.println("-");
}

void ConnectToWIFISpot()
{
  int currentPair = 0;
  while(WiFi.status() != WL_CONNECTED)
  {
    if (currentPair > amountOfSpots - 1)
    {      
      Serial.println("All possible spots is unavailable. Switched to manual mode.");
      return;
    }
    
    //connection
    WiFi.hostname("esp_ap");
    WiFi.begin(spots_essid[currentPair],spots_key[currentPair]);
    WiFi.config(staticIP, subnet, gateway);

    Serial.println("\nLooking for:"); 
    Serial.print("SSID: "); 
    Serial.print(spots_essid[currentPair]);
    Serial.print("\nPassword: ");  
    Serial.print(spots_key[currentPair]); 
    Serial.print("\n");  
  
    int countFailedConnection = 0;
    
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(inputDelayWIFIConnection);
      Serial.print(".");
  
      if (countFailedConnection++ > 10 / amountOfSpots)
      {
        Serial.println("\nFailed to connect.");
        break;
      }
    }
    Serial.println();
    currentPair++;
  }
  Serial.println("Connected. Local network settings:");
  
  Serial.print("IP address: ");           
  Serial.println(WiFi.localIP());
  
  Serial.print("Subnet Mask: ");           
  Serial.println(WiFi.subnetMask());
  
  Serial.print("Gateway IP: ");           
  Serial.println(WiFi.gatewayIP());

  udp.begin(localUdpPort);
  Serial.print("UDP listener port: ");           
  Serial.println(localUdpPort);
  
  Serial.println("---------"); 
  
  f_reset();
  check_board_init();
  SetDefaultSettingsForMode(currentMode);
}

void WIFIConnection()
{
  WiFi.hostname("esp_ap");
  WiFi.begin(essid,key);
  WiFi.config(staticIP, subnet, gateway);
  
  //WiFi.mode(WIFI_STA); //WiFi mode station (connect to wifi router only
  
  Serial.println("\nLooking for:"); 
  Serial.printf("SSID: %s\n", essid); 
  Serial.printf("Password: %s\n", key);  

  int countFailedConnection = 0;
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(inputDelayWIFIConnection);
    Serial.print(".");

    if (countFailedConnection++ > 10)
    {
      Serial.println("\nFailed to connect. Esp changed their own behaviour to waiting.");
      return;
    }
  }
  Serial.println();

  Serial.println("Connected. Local network settings:");
  
  Serial.print("IP address: ");           
  Serial.println(WiFi.localIP());
  
  Serial.print("Subnet Mask: ");           
  Serial.println(WiFi.subnetMask());
  
  Serial.print("Gateway IP: ");           
  Serial.println(WiFi.gatewayIP());

  udp.begin(localUdpPort);
  Serial.print("UDP listener port: ");           
  Serial.println(localUdpPort);
  
  Serial.println("---------"); 
  
  f_reset();
  check_board_init();
  SetDefaultSettingsForMode(currentMode);
}

void f_reset()
{
  digitalWrite(BLUE, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(RED, LOW);  
   
  currentValue = min_value; 
  upwardMovement = true;
  currentWay_heartbeat = 1;
  currentWay_anxiety = 1;
}

void useSpecificMode()
  {    
    switch(currentMode)
    {
      case 0:
        mode_1();
        break;
      case 1:
        mode_2();
        break;
      case 2:
        mode_3();
        break;
      case 3:
        mode_4();
        break;
      case 4:
        mode_5();
        break;
      case 5:
        mode_6();
        break;
      case 6:
        mode_7();
        break;
      case 7:
        mode_8();
        break;
      case 8:
        mode_9();
        break;
      case 9:
        mode_10();
        break;
      case 10:
        mode_11();
        break;
      case 11:
        mode_12();
        break;
      case 12:
        mode_13();
        break;
      case 13:
        mode_14();
        break;
      case 14:
        mode_15();
        break;
      case 15:
        mode_emptiness();
        break;
      default:
        Serial.println("Wrong mode");
        break;
    }
    delay(delay_assign_mode);
  };
  
void check_board_init()
{
  digitalWrite(BLUE, HIGH);
  delay(b_setup_value_ms);
  digitalWrite(BLUE, LOW);
  delay(s_setup_value_ms);  
  digitalWrite(GREEN, HIGH);
  delay(b_setup_value_ms);
  digitalWrite(GREEN, LOW);
  delay(s_setup_value_ms);  
  digitalWrite(RED, HIGH);
  delay(b_setup_value_ms);
  digitalWrite(RED, LOW);
}

void manageValue()
{
  if (upwardMovement)
  {
    currentValue += getStepValue();

    if (currentValue > max_value)
    {
      currentValue = max_value;
      upwardMovement = false;
    }
  }
  else if (!upwardMovement)
  {
    currentValue -= getStepValue();

    if (currentValue < min_value)
    {
      currentValue = min_value;
      upwardMovement = true;
    }
  }
}

void manageValue_Heartbeat()
{
  switch(currentWay_heartbeat)
  {
    case 1:
      currentValue += getStepValue();
  
      if (currentValue > max_value)
      {
        currentValue = max_value;
        currentWay_heartbeat = 2;
      }
      break;
    case 2:
      currentValue -= getStepValue();
  
      if (currentValue < threshold)
      {
        currentValue = threshold;
        currentWay_heartbeat = 3;
      }
      break;
    case 3:
      currentValue += getStepValue();
  
      if (currentValue > max_value)
      {
        currentValue = max_value;
        currentWay_heartbeat = 4;
      }
      break;
    case 4:
      currentValue -= getStepValue();
  
      if (currentValue < min_value)
      {
        currentValue = min_value;
        currentWay_heartbeat = 1;
      }
      break;
    default:
      Serial.println("currentWay_heartbeat error");
  }
}

int getStepValue()
{
  return currentValue > threshold ? speed_step_value : step_value;
}

void setColor(int R, int G, int B)
{
  analogWrite(RED, R);
  analogWrite(GREEN, G);
  analogWrite(BLUE, B);
}

void mode_1()
{
  setColor(BRIGHTNESS, 0, 0);
}

void mode_2()
{
  manageValue();
  setColor(currentValue, 0, 0);  
}

void mode_3()
{
  setColor(0, BRIGHTNESS, 0);
}

void mode_4()
{
  manageValue();
  setColor(0, currentValue, 0);
}

void mode_5()
{
  setColor(0,0, BRIGHTNESS);
}

void mode_6()
{
  manageValue();
  setColor(0,0, currentValue);
}

void mode_7()
{
  //RG
  int r = random(min_value, max_value);
  int g = random(min_value, max_value);
  int b = 0;
  setColor(r,g,b);
}

void mode_8()
{
  //RB
  int r = random(min_value, max_value);
  int b = random(min_value, max_value);
  int g = 0;
  setColor(r,g,b);
  
}

void mode_9()
{
  //GB
  int b = random(min_value, max_value);
  int g = random(min_value, max_value);
  int r = 0;
  setColor(r,g,b);
  
}

void mode_10()
{
  //RGB
  int b = random(min_value, max_value);
  int g = random(min_value, max_value);
  int r = random(min_value, max_value);
  setColor(r,g,b);
  
}

void mode_11()
{
  manageValue_Heartbeat();
  analogWrite(RED, currentValue);
}

void mode_12()
{
  //chill
  manageValue();
  setColor(currentValue, currentValue, max_value);
}

void mode_13()
{
  //rainy
  manageValue();
  setColor(currentValue, currentValue, currentValue);
}

void mode_14()
{
  //falling snow
  manageValue();
  
  switch(currentWay_snow)
  {
    case 1:
      setColor(0,0, currentValue);
      currentWay_snow = 2;
      break;
    case 2:
      setColor(0,0, max_value - currentValue);
      currentWay_snow = 1;
      break;
    default:
      Serial.println("currentWay_snow error");
      break;
  }
}

void mode_15()
{
  //anxiety
  manageValue();
  
  switch(currentWay_anxiety)
  {
    case 1:
      setColor(0,0,currentValue);
      currentWay_anxiety = 2;
      break;
    case 2:
      setColor(0,currentValue,0);
      currentWay_anxiety = 3;
      break;
    case 3:
      setColor(currentValue,0,0);
      currentWay_anxiety = 1;
      break;
    default:
      Serial.println("currentWay_anxiety error");
      break;
  }
}

void mode_emptiness()
{
  f_reset();
}
