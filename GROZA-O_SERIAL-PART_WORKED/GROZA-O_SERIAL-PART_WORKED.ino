uint16_t package[24];

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  Serial.println("Serial initialized..");

  // set the data rate for the SoftwareSerial port
  //mySerial.begin(115200);
}

void loop() { // run over and over
  if (Serial.available()) {
    String recv = Serial.readString();
    Serial.print("Received:");
    Serial.println(recv);
    
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

    Serial.println("String:");
    Serial.println(str);
    
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

    Serial.println("\nArray to send through UDP channel:");    
    for(int i = 0; i < sizeof(package)/sizeof(package[0]); i++)
    {
      Serial.print(package[i]);
      Serial.print(" ");
    }
    Serial.println("");
  }
  
}

