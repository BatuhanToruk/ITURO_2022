//Robotistan.com L298 motor sürücü kartı örnek kodu.

#include <Servo.h>

Servo myservo;


int mes = 0;
int sensor = A1;
int INB = 9;   //MotorA çıkışı için pwm kontrol pini
int ENA = 6;  //MotorB çıkışı için pwm kontrol pini
int INA = 10;  //MotorA çıkışı için yön kontrol pini
int ENB = 11;  //MotorB çıkışı için yön kontrol pini
int a=-15;
int hiz = 0;   //hız değeri
int buzz = 2;
int kp=10;
int kd=30;
int P=0,D=0,PD=0;
int delta=0;
int hata=0,hata0=0;
int data=0;
int max_h=240;
int bsl = 0;
int but=A0;
int but_s;
int d1=13, d2=12,d3=8,d4=7,d5=5;
int ds1=0, ds2=0,ds3=0,ds4=0,ds5=0;
int datapin=4;

void dataOku(){
ds1=digitalRead(d1);ds2=digitalRead(d2);ds3=digitalRead(d3);ds4=digitalRead(d4);ds5=digitalRead(d5);
//digitalWrite(L5,ds5);digitalWrite(L4,ds4); digitalWrite(L3,ds3); digitalWrite(L2,ds2); digitalWrite(L1,ds1);
hata=ds4*8+ds3*4+ds2*2+ds1;
if(ds5==1)hata=-hata;
//Serial.print("Hata:");
//Serial.println(hata);
}

void motor(signed int sol, signed int sag) {
  if (sol >= 0) {
    digitalWrite(INA, LOW);
    analogWrite(ENA, sol);
  }
  else {
    digitalWrite(INA, HIGH);
    analogWrite(ENA, 255+sol);
  }
  if (sag >= 0) {
    digitalWrite(INB, LOW);
    analogWrite(ENB, sag);
  }
  else {
    digitalWrite(INB, HIGH);
    analogWrite(ENB, 255+sag);
  }
}

void aci(signed int aci)
{
  motor(250 + aci, 250-aci);
 // if(aci<0)  motor(200 + aci, 200-aci);
  //else  motor(200+aci , 200- aci);
}
void setup()
{
  pinMode(ENA, OUTPUT);  //Kontrol pinleri çıkış olarak ayarlanır
  pinMode(ENB, OUTPUT);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
 pinMode(buzz, OUTPUT);
 
  pinMode(d1, INPUT);pinMode(d2, INPUT);pinMode(d3, INPUT);pinMode(d4, INPUT);pinMode(d5, INPUT);
  pinMode(datapin,OUTPUT);
  pinMode(but, INPUT_PULLUP);
  analogWrite(ENA, 0);  //İki motor da durur
  analogWrite(ENB, 0);
  Serial.begin(9600);
  pinMode(sensor, INPUT);
  myservo.attach(3);
  myservo.write(70);
  motor(0,0);
//
//  but_s=digitalRead(but);
//  while(but_s){but_s=digitalRead(but);}
//
//  for(int i=0;i<3;i++){
//    digitalWrite(buzz,1);delay(200);
//    digitalWrite(buzz,0);delay(200);
//    }

}



void buzOt(int ot){
  for(int i=0;i<ot;i++){
    digitalWrite(buzz,1);delay(200);
   digitalWrite(buzz,0);delay(200);
   }}
void kamera()
{
  mes = digitalRead(sensor);
  
  if (mes == 0){
    motor(0,0);
    myservo.write(160);
    digitalWrite(datapin,1);
    delay(4000);
    digitalWrite(datapin,0);
    while(1);
    while(hata==0){dataOku();Serial.println(hata);motor(0,0);}
    digitalWrite(datapin,0);
    while(1);
    delay(1000);
    myservo.write(70); 
    if(hata==15) {buzOt(1);motor(90,150);delay(5500);motor(0,0);}
    else if(hata==-12){buzOt(3);motor(90,150);delay(2850);motor(35,125);delay(4000);motor(0,0);}
    else if(hata==12){buzOt(2);motor(100,145);delay(2200);motor(140,40);delay(3000);motor(0,0);}
    

    
  }
  else  { myservo.write(70); 
  digitalWrite(datapin,0);
  } 

}
void loop() {
dataOku();
kamera();
    
if(hata == -15){
      motor(0, 0);
    }
else if(abs(hata)<12){

    P=kp*hata;
    delta=hata-hata0;
    D=kd*delta;
    PD=P+D;
    if(PD>max_h) PD=max_h;
    if(PD<(-max_h)) PD=-max_h;
   
      aci(PD);
}
else {
  
  
  }



    //Serial.println(PD);
    //Serial.println(PD);
  //}
}
