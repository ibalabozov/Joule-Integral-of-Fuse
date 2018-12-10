// Библиотека за LCD дисплея.
#include <LiquidCrystal.h>
// Деклариране на пиновете към, които е свързан екрана.
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
// Деклариране на променливи за използваните 
// пинове и нужни записи на различни стойности.
int relay = 7;
int mosfet = 6;
int sensor = A0;
int i = 0;
int delayTime = 0;
int gate = 0;
int transistorGate;
float Realcurrent;
String data = "";


void setup() {
  // Казва се на микроконтролера, че напреженовият сигнал,
  // който се получава на входа на аналоговите пинове
  // ще се сравнява с напрежението подадено от външен източник.
  analogReference(EXTERNAL);
  analogRead(sensor);
  // Казва се какъв вид е използвания пин(вход или изход), деклариран от променливите.
  pinMode ( relay, OUTPUT);
  pinMode(mosfet, OUTPUT);
  // Започване на комукация със серийния порт и LCD дисплея.
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setCursor(0, 0);
  lcd.print("Choose option");
  lcd.setCursor(0, 1);
  lcd.print("from MATLAB");
}


void loop() {
  // Изчакване на команда от серийния порт.
  while (Serial.available() == 0);
  int command = Serial.read() - '0';
  // Условие, което се изпълнява при получена команда '2'.
  if (command == 2){
    // Изчаква получаването на избрания проспектен ток.
    // При различен избран проспектен ток
    // се записват различни стойности за
    // време през, което се прави запис от сензора, 
    //сигнал за отпушване на транзистора
    // и променлива за визуализация на реалния избран ток на екрана.
    while (Serial.available() == 0);
     transistorGate = Serial.read();
   if (transistorGate == 0){
    Realcurrent = 0;
    gate =0; 
   }
   else if ( transistorGate == 25){
    Realcurrent = 0.5;
    delayTime = 4;
    gate = 25;
   }
   else if ( transistorGate == 52){
    Realcurrent = 1;
     delayTime = 4;
     gate = 52;
   }
   else if ( transistorGate == 81){
    Realcurrent = 1.5;
     delayTime = 4;
     gate = 81;
   }
   else if ( transistorGate == 110){
    Realcurrent = 2;
     delayTime = 4 ;
     gate = 100;
   }
   else if (transistorGate == 139){
    Realcurrent = 2.5;
     delayTime = 3;
     gate = 139;
   }
   else if (transistorGate == 168){
    Realcurrent = 3;
     delayTime = 2;
     gate = 168;
   }
   else if (transistorGate == 197){
    Realcurrent = 3.5;
     delayTime = 2;
     gate = 197;
   }
   else if (transistorGate == 220){
     Realcurrent = 4;
     delayTime = 2;
     gate = 220;
   }
   else if (transistorGate == 255){
    Realcurrent = 4.5;
     delayTime = 2;
     gate = 255;
   }
   // Визуализация на екрана.
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("The prospective");
    lcd.setCursor(0,1);
    lcd.print("current is " + String(Realcurrent) +"A");
  }
  // Условие за изпълнение при получаване на команда '3';
  if (command == 3){
  // Транзистора се отпушва със сигнал зависещ от избрания проспектен ток.
  analogWrite(mosfet, gate);
  // Изпълнение на цъкъл за бързо отчитане на сигнала от сензора, измерване на времето 
  // и записване на резултатие в паметта на Ардуино.
   sense();

 // Изращане на масива от информация събран от цикъла за измерване
 // през серийния порт.
Serial.println(data);
Serial.println("/");
// Зануляване на масива.
String data = "0";
// Визуализация на екрана, че вложката е изгоряла.
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("The fuse");
lcd.setCursor(0, 1);
lcd.print("is blown");
// Отваряне на релето и запушване на транзистора.
digitalWrite(relay, LOW);
analogWrite(mosfet , 0);
  }
  // Условие за изпълнение при получена команда '4'.
  if (command == 4) {
  // Визуализация на екрана да се избере опция от MATLAB.
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Choose option");
  lcd.setCursor(0, 1);
  lcd.print("from MATLAB");
  }
  }
 // Цикъл за измерване.
void sense(){
 //Стартиране на броене на времето.
  unsigned long t1=millis();
 // Цикъл, който прави 20 измервания преди да затвори веригата на предпазителя.
 for ( int i=0; i<20; i++)
 { 
  // Четене от сензора.
  int val = analogRead(sensor);
  // Запис на резултатие в масив.
  String timer = String(millis()-t1);
  data += timer + "," + String(val)+"\n";
  delay(delayTime);
}
// Затваряне на релето.
digitalWrite(relay, HIGH);
//Цикъл за измерване и записване на сигнала от сензора
// и времето през, което протича ток през вложката, в масив в паметта на Ардуино.
 for ( int i=21; i<200; i++)
 { 
  int val = analogRead(sensor);
  String timer = String(millis()-t1);
  data += timer + "," + String(val)+"\n";
  delay(delayTime);
  }  
} 
