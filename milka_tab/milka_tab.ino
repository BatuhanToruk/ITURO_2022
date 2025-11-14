#include <Servo.h>

Servo servo;
Servo servo1;
//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(12, 13); // RX, TX
//


//int hedef=[0,0,0];


byte s1 = 35, s2 = 37, s3 = 39, s4 = 41, s5 = 43, s6 = 45, s7 = 47, s8 = 49; //Sensörler
byte buton = 52; // Açma Kapaama Düğmesi

byte buzz = 50; //buzzer
byte bts;
int s_s1 = 0, s_s2 = 0, s_s3 = 0, s_s4 = 0, s_s5 = 0, s_s6 = 0, s_s7 = 0, s_s8 = 0; //Sensör İlk Değeri 0 Oldu

const int kp = 20;
const int kd = 35;
const int maxt = 210;
const int gecis = 250;
int gelen[]={32,73,41,53,34,61,43,72,11,51,21,62,22,52,23,82};
//int gelen[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//--------Köse Takip için----------------------------------------------

int syc1 = 0, syc2 = 0, error1 = 0, error2 = 0;

//---------------------------------------------------------------------

//--------Motor Çıkışları-----------------------------------------
int ENB = 10;   //MotorA çıkışı için pwm kontrol pini
int ENA = 12;  //MotorB çıkışı için pwm kontrol pini
int INB = 11;  //MotorA çıkışı için yön kontrol pini
int INA = 13;  //MotorB çıkışı için yön kontrol pini
int hiz = 0;   //hız değeri
//-------------------------------------------------------
int tabsay=9;

int dt=0,m=0;
int hata = 0, bir = 0, on = 0;
int hata1 = 0;
int delta_hata = 0;
int total = 0;
int sag1 = 0;
int sol1 = 0;
int syc = 0;
int pist = 1;

int i = 1,i_eski=1, P = 0, D = 0, j = 0;



void ekrana_bas() // Method Sensor Değerlerini Ekrana Basıyor
{
 // sensorRead();
  Serial.print("sensorler=  ");
  Serial.print(s_s1);
  Serial.print(s_s2);
  Serial.print(s_s3);
  Serial.print(s_s4);
  Serial.print(s_s5);
  Serial.print(s_s6);
  Serial.print(s_s7);
  Serial.print(s_s8);
  Serial.print("  P=");
  Serial.print(P);
  Serial.print("  D=");
  Serial.print(D);
  Serial.print("  hata1=");
  Serial.print(hata1);
  Serial.print("  sayac=");
  Serial.print(syc);
  Serial.print("  sol=");
  Serial.print(sol1);
  Serial.print("  sag=");
  Serial.print(sag1);
  Serial.print("  hata=");
  Serial.println(hata);
}



void setMotor(signed int sol, signed int sag)
{

  if (sol >= 0)
  {
    analogWrite(ENA, sol);
    digitalWrite(INA, LOW);
  }
  else
  {
    analogWrite(INA, abs(sol));
    digitalWrite(ENA, LOW);
  }
  if (sag >= 0)
  {
    analogWrite(ENB, sag);
    digitalWrite(INB, LOW);
  }
  else
  {
    analogWrite(INB, abs(sag));
    digitalWrite(ENB, LOW);
  }

}


void sagaDon()
{
  sensorRead();
  while (s_s8 != i) {         // 8.sensör Siyah Çizğiyi Görmediği Sürece Saga Dön
    s_s8 = digitalRead(s8);
    setMotor(140, -205);
  }
  //delay(60);
  sensorRead();
  while (s_s4 != i) {
    s_s4 = digitalRead(s4);
    setMotor(80, -125);
  } 
  //delay(60);                 // 4.Sensör Siyah Çizğiyi Görmediği Sürece YAVAŞÇA Dön
  setMotor(0, 0); delay(500);
}

void solaDon()
{
  sensorRead();
  while (s_s1 != i) {
    s_s1 = digitalRead(s1);
    setMotor(-140, 205);
  }
  delay(50);
  sensorRead();
  while (s_s5 != i) {
    s_s5 = digitalRead(s5);
    setMotor(-100, 155);
  } delay(20);
  setMotor(0, 0); delay(500);
}

void sensorRead()
{
  //pist_belirle();
  hata1 = hata;
  hata = 0; syc = 0;
  s_s1 = digitalRead(s1); if (s_s1 == i) {hata = hata - 7;syc++;}
  s_s8 = digitalRead(s8); if (s_s8 == i) {hata = hata + 7;syc++;}
  s_s2 = digitalRead(s2); if (s_s2 == i) {hata = hata - 5;syc++;}
  s_s3 = digitalRead(s3); if (s_s3 == i) {hata = hata - 3; syc++;}
  s_s4 = digitalRead(s4); if (s_s4 == i) {hata = hata - 1; syc++;}
  s_s5 = digitalRead(s5); if (s_s5 == i) {hata = hata + 1;syc++;}
  s_s6 = digitalRead(s6); if (s_s6 == i) {hata = hata + 3;syc++;}
  s_s7 = digitalRead(s7); if (s_s7 == i) {hata = hata + 5;syc++;}
  
  if (syc > 0)hata = hata / syc;
  else
  {
    if (hata1 >= 0)hata = 10;
    if (hata1 < 0)hata = -10;
  }
  delta_hata = hata - hata1;
  P = hata * kp;
  D = delta_hata * kd;
  total = P + D;
  if (total > maxt) total = maxt;
  if (total < -maxt) total = -maxt;
}


void cizgiTakip()
{
  sensorRead();
  sol1 = (140 + total); sag1 = (205 - total);//140  205
  if (sag1 < -255) sag1 = -255; if (sag1 > 255) sag1 = 255;
  if (sol1 < -255) sol1 = -255; if (sol1 > 255) sol1 = 255;
  setMotor(sol1, sag1);
}

void azGit(int mes)
{
 setMotor(140, 205); delay(mes); 
setMotor(-70,20);delay(50);setMotor(0,0);
  }
 void buzOt(){
for(int k=0;k<3;k++){digitalWrite(buzz, HIGH);delay (300);digitalWrite(buzz, LOW);delay (300);}
 }
// void dataBekle(){
//  int dt=0;
//  while(dt==0){
//   if (Serial.available()) {
//    int inByte = Serial.parseInt();
//    if (inByte==9) dt=1;
// 
//    
//   }}
// }
void dataBekle(){
  dt=0;setMotor(0,0);
  while(dt==0){
    if (Serial.available()>0){
      digitalWrite(buzz,1);
     int tab = Serial.parseInt();
     //Serial.println(tab);
      if (tab==9) dt=1;
      else if(tab>10){
        gelen[m]=tab;
         m++;
      }
     
      if(m>15) {m=0;dt=1;}
    }
    digitalWrite(buzz,0);
  }}
 
  void dataGonder(){
    for(int k=0;k<16;k++){
      digitalWrite(buzz,1);
  Serial1.println(gelen[k]);
  delay(200); digitalWrite(buzz,0);delay(200);
  }
  Serial1.println(99);
 // Serial.println("gonderildi");
  }
    
  
  
  
void setup() {
  //----Motor Pinleri Çıkışı---------
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  //-----------------------------
  setMotor(0,0);
  pinMode(buzz, OUTPUT);
  
  pinMode(buton, INPUT_PULLUP);
  // Initialize all pins as low:
  
  //------Sensörler----------
  pinMode(s1, INPUT);
  pinMode(s2, INPUT);
  pinMode(s3, INPUT);
  pinMode(s4, INPUT);
  pinMode(s5, INPUT);
  pinMode(s6, INPUT);
  pinMode(s7, INPUT);
  pinMode(s8, INPUT);
  //-------------------------
  //mySerial.begin(9600);
  Serial.begin(9600);
  Serial1.begin(9600);  //bluetooth Haberleşme

  //------Eğer Buton Açık/Kapalı Kontrölü---------------
/*dataBekle();
for(int k=0;k<85;k++) Serial1.write(100);
delay(1000);

dataGonder();
while(1);*/

delay(3000);
//while(1)setMotor(140,205);
 bts = digitalRead(buton);
while(bts) bts=digitalRead(buton);
delay(1000);
//dataGonder();
//while(1);
//while(1){ sensorRead();cizgiTakip();}

for(int k=0;k<2;k++){digitalWrite(buzz, HIGH);delay (300);digitalWrite(buzz, LOW);delay (300);}

// setMotor(140,205);
//while(1);
    
delay(1000);

  
}

void loop(){
//i=0;
int tab=1;
//sensorRead();
//while(syc<4) cizgiTakip();
//azGit(350);
//delay(3000);
setMotor(140,205);delay(700);setMotor(0,0);delay(300);
i=1;
while(tab<tabsay){
    sensorRead();
    while(syc<4) cizgiTakip();
     azGit(250);
     
   // buzOt();
    Serial.write("9");
   dataBekle();
    //delay(2000);
    tab++;
    //resim cek delay
}
setMotor(0,0);delay(300);
sagaDon();
sagaDon();
tab=1;
while(tab<tabsay){
    sensorRead();
    while(syc<4) cizgiTakip();
    azGit(250);
    tab++;
    
}
azGit(400);
buzOt();
Serial.write("8");
dataBekle();
buzOt();
//for(int k=0;k<85;k++) Serial1.println(100);
delay(500);
dataGonder();

while(1);



}
