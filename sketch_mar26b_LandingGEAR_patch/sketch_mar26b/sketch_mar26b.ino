//
//               ***   version.6_last (рабочий вариант, но без одновременного поднятия шасси)  ***
//
// 1ms - down; 2ms - up
// cal 1.5ms - start; 2.1ms - end

int c_main = 2;   // сигнал управления шасси с борта
int c_left = 11;  
int c_right = 9;

int c_dc = 6;    // сигнал управления питанием на сервоприводы

int gain_in = 5;   // S3 переключатель
int gain_out = 10; // сигнал управления передатчиком

int x[99];             
int check = 0;   
int spd = 12.5; // скорость вращения вала сервопривода в попугаях

unsigned long val_mode;
unsigned long width;
unsigned long diff;
unsigned long sw;

void setup() {
pinMode(c_main,INPUT);
pinMode(gain_in,INPUT);

pinMode(c_right,OUTPUT);
pinMode(c_left,OUTPUT);
pinMode(c_dc,OUTPUT);
pinMode(gain_out,OUTPUT);

Serial.begin(9600);
}

void loop() {
  digitalWrite(c_dc,LOW);    // выключение питания сервоприводов
  do {} while (millis() < 5000);   // плата ждет 5 секунд во избежание рандомных значений ширин импульса шасси коптера
  
  int i = 0;
  while (1) {  
   // управление усилителем
    sw = pulseIn(gain_in,HIGH); // switcher s3            считываются значения ширин импульсов тумблера s3
    if (sw > 1700) {
      digitalWrite(gain_out,HIGH);
      delay(3000);                 // на 3 секунды включает усилитель. чтобы он работал непрерывно, достаточно постоянно держать тумблер в верхнем положении
      digitalWrite(gain_out,LOW); 
    } else {      
      digitalWrite(gain_out,LOW); 
    } //конец. управление усилителем
    
   // управление шасси
   width = pulseIn(c_main,HIGH);    // считываются значения ширины импульсов с рычажка пульта
   x[i] = width;      
   if (i >= 1) {
   
      if ((x[0] > -100 && x[0] < 100) || (x[i-1] < 100 && x[i-1] > -100)) { // защита от неверных значений ширин импульса при запуске системы
        break;
      } 
      
      if (abs(x[i]-x[i-1])>700 && abs(x[i]-x[i-1])<1200){ //при переключении свитча подается напряжение на сервоприводы
        if ((x[i-1]>=1800 && x[i]>=1800)||(x[i-1]<=1200 && x[i]<=1200)){ // защита от одинаковых двух последовательных значений ширин импульсов
          break; 
        }        
        digitalWrite(c_dc,HIGH);       // подается питание на сервоприводы  
        
        if (width > 1700 && width < 2200) {  
            left_up();
            right_up();
        } else if (width < 1200 && width > 800) {
            left_down();
            right_down();   
        } else {    
            digitalWrite(c_dc,LOW);
        }
    
      } // конец подачи напряжения на сервоприводы
   } // конец условия if (i >= 1)
  i++;
  } // конец цикла while  
}

void left_up() {
  for (int i = 1000; i <= 2000; i = i + spd) {
          digitalWrite(c_left,HIGH);
          delayMicroseconds(i);
          digitalWrite(c_left,LOW);
          delay(20);
        }
}

void right_up() {
  for (int i = 2000; i >= 1000; i = i - spd) {
            digitalWrite(c_right,HIGH);
            delayMicroseconds(i);
            digitalWrite(c_right,LOW);
            delay(20);
          }
}

void left_down() {
  for (int i = 2000; i >= 1000; i = i - spd) {
          digitalWrite(c_left,HIGH);
          delayMicroseconds(i);
          digitalWrite(c_left,LOW);
          delay(20);
        }
}

void right_down() {
  for (int i = 1000; i <= 2000; i = i + spd) {
          digitalWrite(c_right,HIGH);
          delayMicroseconds(i);
          digitalWrite(c_right,LOW);
          delay(20);
        }
}

