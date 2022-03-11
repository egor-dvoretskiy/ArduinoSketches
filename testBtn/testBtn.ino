#include <Bounce2.h>
 
#define pressed_long 2000 // долговременное нажатие = 2 секунды
#define num_modes 5 // максимальный номер режима
 
short int max_mode = num_modes + 1; // вспомогательная переменная
 
Bounce bouncer = Bounce(); //создаем экземпляр класса Bounce
 
unsigned long pressed_moment; // момент нажатия кнопки
int current_mode = 0; // текущий режим
 
void setup()
{
 pinMode(14 ,INPUT); // кнопка на пине 2
 digitalWrite(14 ,HIGH); // подключаем встроенный подтягивающий резистор
 bouncer .attach(14); // устанавливаем кнопку
 bouncer .interval(5); // устанавливаем параметр stable interval = 5 мс
 Serial.begin(115200); //установка Serial-порта на скорость 9600 бит/сек
}
 
void loop()
{
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
    current_mode++; // увеличиваем счетчик текушего режима
    current_mode %= max_mode; // остаток от целочисленного деления
    if (current_mode == 0) current_mode = 1; // режим меняется от 1 до num_modes
   }
   else
   { // кнопка удерживалась долго
    current_mode = 0; 
    pressed_moment = 0; // обнуляем момент нажатия
   }
   Serial.println("Current mode:");
   Serial.println(current_mode); // выводим сообщение о текущем режиме
  }
 }
}
