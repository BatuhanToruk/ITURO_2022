//Robotistan.com L298 motor sürücü kartı örnek kodu.

#include <Servo.h>

Servo myservo;
int L1=43, L2=45,L3=47,L4=49,L5=53;
int d1=52, d2=50,d3=48,d4=46,d5=44;
int ds1=0, ds2=0,ds3=0,ds4=0,ds5=0;
int datapin=42;
int mes = 0;
int syc=0;
int sensor = 8;
int ENA = 10;   //MotorA çıkışı için pwm kontrol pini
int ENB = 11;  //MotorB çıkışı için pwm kontrol pini
int INA = 12;  //MotorA çıkışı için yön kontrol pini
int INC = 13;  //MotorB çıkışı için yön kontrol pini
int a=-15;
int hiz = 0;   //hız değeri
int buzz = 4;
int kp=10;
int kd=20;
int P=0,D=0,PD=0;
int delta=0;
int hata=0,hata0=0;
int data=0;
int max_h=250;
int bsl = 0;
int but=A0;
int but_s;
int sure=500;

void motor(signed int sol, signed int sag) {
  if (sol >= 0) {
    digitalWrite(INA, HIGH);
    analogWrite(ENA, sol);
  }
  else {
    digitalWrite(INA, LOW);
    analogWrite(ENA, abs(sol));
  }
  if (sag >= 0) {
    digitalWrite(INC, HIGH);
    analogWrite(ENB, sag);
  }
  else {
    digitalWrite(INC, LOW);
    analogWrite(ENB, abs(sag));
  }
}

void aci(signed int aci)
{
  if(aci<0)  motor(200 + aci, 200);
  else  motor(200 , 200- aci);
}
void setup()
{
  pinMode(ENA, OUTPUT);  //Kontrol pinleri çıkış olarak ayarlanır
  pinMode(ENB, OUTPUT);
  pinMode(INA, OUTPUT);
  pinMode(INC, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(L1, OUTPUT);pinMode(L2, OUTPUT);pinMode(L3, OUTPUT);pinMode(L4, OUTPUT);pinMode(L5, OUTPUT);
  pinMode(d1, INPUT);pinMode(d2, INPUT);pinMode(d3, INPUT);pinMode(d4, INPUT);pinMode(d5, INPUT);
  pinMode(datapin,OUTPUT);
  pinMode(but, INPUT_PULLUP);
  analogWrite(ENA, 0);  //İki motor da durur
  analogWrite(ENB, 0);
  Serial.begin(9600);
  Serial1.begin(9600);
  digitalWrite(datapin,0);
  pinMode(sensor, INPUT);
  myservo.attach(9);
 myservo.write(60);
  motor(0,0);
  //while(1);

//  but_s=digitalRead(but);
//  while(but_s){but_s=digitalRead(but);}
//  for(int i=0;i<3;i++){
//    digitalWrite(buzz,1);delay(200);
//    digitalWrite(buzz,0);delay(200);
//    }
//     motor(100,100);
//     while(1);
//  while(bsl==0){
//    if (Serial.available() > 0) {
//    hata0=hata;
//    data = Serial.parseInt();
//    if(data > 700){
//     bsl = 1; 
//     }  
//    }
//  }
}
void cizgiTakip(){
dataOku();
    P=kp*hata;
    delta=hata-hata0;
    D=kd*delta;
    PD=P+D;
    if(PD>max_h) PD=max_h;
    if(PD<(-max_h)) PD=-max_h;
   
      aci(PD);
  
}

void dataOku(){
ds1=digitalRead(d1);ds2=digitalRead(d2);ds3=digitalRead(d3);ds4=digitalRead(d4);ds5=digitalRead(d5);
digitalWrite(L5,ds5);digitalWrite(L4,ds4); digitalWrite(L3,ds3); digitalWrite(L2,ds2); digitalWrite(L1,ds1);
hata=ds4*8+ds3*4+ds2*2+ds1;
if(ds5==1)hata=-hata;
//Serial.print("Hata:");
//Serial.println(hata);
}
void buzOt(int ot){
  for(int i=0;i<ot;i++){
    digitalWrite(buzz,1);delay(200);
   digitalWrite(buzz,0);delay(200);
   }}
void sureliCizgi(int kk){
  syc=0;
while(syc<kk){
      hata=0;
      cizgiTakip();
      syc++;
      delayMicroseconds(250);
      }
      
      
      }
void kamera()
{
  mes = digitalRead(sensor);
 syc=0;
 int yon=0;
  if (mes == 0){
    motor(0,0);
    myservo.write(140);
    digitalWrite(datapin,1);
    delay(1000);
    dataOku();
    while(hata!=0){dataOku();}//digitalWrite(buzz,1);}
    dataOku();//digitalWrite(buzz,0);
    delay(1000);
    dataOku();
   // buzOt(abs());
    if(hata==15) {buzOt(1); yon=1;}
    else if(hata==12) {buzOt(2);yon=2;}
    else if(hata==-12) {buzOt(3);yon=3;}
    else if(hata==14) {buzOt(7);yon=7;}
    else if(hata==-13) {buzOt(4);yon=4;}
    else if(hata==13) {buzOt(6);yon=6;}
    else if(hata==-14) {buzOt(5);yon=5;}
    else if(hata==0) {yon=0;}
    //while(1);
    myservo.write(60); 
    digitalWrite(datapin,0);
    motor(100,100); delay(100);
    
      //while(1);
 

    if(yon==1) {sureliCizgi(7000);motor(0,0);delay(sure);motor(190,210);delay(4000);motor(0,0);}
    else if(yon==3){sureliCizgi(8000);motor(0,0);delay(sure); motor(108,225);delay(4200);motor(0,0);}
    else if(yon==4){sureliCizgi(8000);motor(0,0);delay(sure); motor(100,225);delay(4000);motor(0,0);while(1);}
    else if(yon==2){sureliCizgi(8000);motor(0,0);delay(sure);}
    else if(yon==6){sureliCizgi(18000);motor(0,0);delay(sure);while(1);}
    else if(yon==7){sureliCizgi(8000);motor(0,0);delay(sure);motor(175,210);delay(4000);motor(0,0);delay(100);sureliCizgi(12000);motor(0,0);while(1);}
    else if(yon==5){sureliCizgi(8000);motor(0,0);delay(sure);motor(180,210);delay(4200);motor(0,0);delay(3000);motor(100,225);delay(4000);motor(0,0);while(1);}
    else if(yon==0){sureliCizgi(9000);motor(0,0);delay(sure);}
  }
  else  { myservo.write(60); 
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

cizgiTakip();
}
else {
  
  
  }



    //Serial.println(PD);
    //Serial.println(PD);
  //}
}
