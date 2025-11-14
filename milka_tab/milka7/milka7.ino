#include <Servo.h>

//-----------Grapper-------------------------
Servo servo; 
Servo servo1;

byte kutu_durum=0; //Kutu Aldıysa 1 Kutu Almadıysa 0
int h_knt=1; //Bluetooth Üzerinden Serial Sıkışmasını  Engellemek için 
int b_knt=0; //?? Satir ve sutunu azaltmaması için mi?
byte s1 = 34, s2 = 36, s3 = 38, s4 = 40, s5 = 42, s6 = 44, s7 = 46, s8 = 48 ; //Sensörler
byte buton = 22;// Açma Kapaama Düğmesi Durumu
byte buzz = 4; //buzzer
byte bts; // Açma Kapaama Düğmesi Durumu
int s_s1 = 0, s_s2 = 0, s_s3 = 0, s_s4 = 0, s_s5 = 0, s_s6 = 0, s_s7 = 0, s_s8 = 0; //Sensör Default Değeri 0 Oldu
int s_kod=9;
int s_s9 = 0;
//--------------Başlangıç--Konumu-------------------------------------
int yon = 0;
int satir = 5;
int sutun = 5;
int led=50;
//---------Hedef--Konum----------------------------------------------
byte birak[] = {32,  32,  34,  11,  34, 42, 34, 44};
int al[] =     {42,  32,  12,  33,  22, 41, 43, 21};
int gelen[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int al1[]={0,0,0,0,0,0,0,0};
int birak1[]={0,0,0,0,0,0,0,0};
int temp=0 ;
int G_sutun=1;
int H_satir = 1;
int H_sutun = 1;  //Değişken Açmak için Default Değer Verildi
int H_Alt=1;
int m=0;

//--------------------------------------------------------------------

//----------Çizgi--Takip------------------
const int kp = 12;    // 25   PID Motor Controller 
const int kd = 20;    // 50
const int maxt = 170;
const int gecis = 250;
//-----------------------------------

//--------Motor Çıkışları-----------------------------------------
int ENA = 10;   //MotorA çıkışı için pwm kontrol pini
int ENB = 11;  //MotorB çıkışı için pwm kontrol pini
int INA = 12;  //MotorA çıkışı için yön kontrol pini
int INB = 13;  //MotorB çıkışı için yön kontrol pini
int hiz = 0;   //hız değeri
//-------------------------------------------------------
int kareKonum=1;
int oku;
int hata = 0, bir = 0, on = 0;
int hata1 = 0;
int delta_hata = 0;
int total = 0;
int sag1 = 0;
int sol1 = 0;
int syc = 0;
int pist = 0;
int pt = 0, ppt = 0;
int P = 0, D = 0, j = 0;
int tab;
int g_syc=0; //Görev Sayacı 
int i = 1;
int cksGit=0;
int kutuSayisi=7;
void setup() {
	//Servo Pini Tanımlaması------
	servo.attach(6);
  servo1.attach(5); //----Motor Pinleri Çıkışı---------
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(s1, INPUT);  pinMode(s2, INPUT);  pinMode(s3, INPUT);  pinMode(s4, INPUT);  pinMode(s5, INPUT);  pinMode(s6, INPUT);  pinMode(s7, INPUT);  pinMode(s8, INPUT);
  pinMode(buzz, OUTPUT);
  pinMode(buton, INPUT_PULLUP);
 
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(9600);  //bluetooth Haberleşme
  servo.write(10); servo1.write(170);
	//------Eğer Buton Açık/Kapalı Kontrölü---------------
	bts = digitalRead(buton);
	 //Açılışta Grapper Açık Şekide Duracak
// while(1){ 
// servo.write(10); servo1.write(170);delay(1000);
//servo.write(130); servo1.write(50);delay(1000);}
//

// EPROMDAN AL VE BIRAKMA OKU

//
dataBekle();
//
//	while(bts) bts=digitalRead(buton);
//	for(int t=0;t<3;t++){
//		digitalWrite(buzz,1); delay(300);
//		digitalWrite(buzz,0); delay(300); 
//	} 
 // while(1){sensorRead();ekranaBas();}
//    servo.write(130); servo1.write(50);delay(1000);
//   servo.write(10); servo1.write(170);delay(1000);
//    }


  sirala();
  siraYazdir();
  //while(1);
	giris();
	gorevAta();
	delay(1000);
	}
void loop(){
	if(b_knt==1) kavsak();
	
	cizgiTakip();
	if(syc>4){kavsak();	} 
	}

void buzOt(int m){
    for(int t=0;t<m;t++){
    digitalWrite(buzz,1); delay(300);
    digitalWrite(buzz,0); delay(300); 
  } 
  
  }
void dataBekle(){
	oku=0;setMotor(0,0);
  Serial.println("data bekliyor :");
	while(oku==0){
    
		if (Serial1.available()>0){
      digitalWrite(buzz,1);
		 tab = Serial1.parseInt();
     //Serial.println(tab);
     if(tab!=0){
        gelen[m]=tab;
         m++;
      }
     Serial.println(tab);
     
			if(tab==99) {m=0;oku=1;}
     //oku=1;
		}
   digitalWrite(buzz,0);
	}
   Serial.print("Gelen :");
   for(int s=0;s<16;s++){Serial.print(gelen[s]);}
   Serial.println("");
   for(int s=0;s<8;s++){
    birak[s]=gelen[2*s];
    al[s]=gelen[2*s+1]-40;
   // BIRAK VE ALDAKİ HERSEYİ EEPROMA YAZ///
  

 
   }
   
	Serial.print("Al    :");
   for(int s=0;s<8;s++){Serial.print(al[s]);Serial.print(",");}
   Serial.println("");
   Serial.print("Birak :");
    for(int s=0;s<8;s++){Serial.print(birak[s]);Serial.print(",");}
   Serial.println("");
	
	} 
void siraYazdir(){
	Serial.print("Al     :[");
	for (int k=0; k<8;k++){Serial.print(al[k]);Serial.print(", ");}
	Serial.println("]");
	Serial.print("birak:[");
	for (int k=0; k<8;k++){Serial.print(birak[k]);Serial.print(", ");}
	Serial.println("]");}
 void siraYazdir_b(){
 Serial1.print("Al     :[");
  for (int k=0; k<8;k++){Serial1.print(al[k]);Serial1.print(", ");}
  Serial1.println("]");
  Serial1.print("birak:[");
  for (int k=0; k<8;k++){Serial1.print(birak[k]);Serial1.print(", ");}
  Serial1.println("]");}
void sirala(){
	for (int k=0; k<8;k++){
		for (int j=k+1;j<8;j++){
			if(birak[k] > birak[j]){    
				temp = birak[k];  
				birak[k] = birak[j];    
				birak[j] = temp;    
				temp=al[k];
				al[k] = al[j];    
				al[j] = temp;
			}
		}
  }}
void ekranaBas(){
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
	Serial.println(hata);}
void sagaDon(){
  sensorRead();
  setMotor(0, 0); delay(500);
  while (s_s8 != i) {         // 8.sensör Siyah Çizğiyi Görmediği Sürece Saga Dön
    s_s8 = digitalRead(s8);
    setMotor(220, -220);
  }
    sensorRead();
 while(syc>2) {sensorRead();setMotor(200, -200);}
  delay(50);
  sensorRead();
	while (s_s4 != i) {s_s4 = digitalRead(s4);setMotor(150, -150);} 
	delay(40);                 // 4.Sensör Siyah Çizğiyi Görmediği Sürece YAVAŞÇA Dön
	setMotor(0,0);delay(200);
	yon ++;                     //Yön arttırıldı ve Eğer yön 4 olursa 360 olduğu için 0 Oldu
	if (yon == 4)  yon = 0;}
void solaDon(){
  sensorRead();
  setMotor(0, 0); delay(500);
  while (s_s1 != i) {
    s_s1 = digitalRead(s1);
    setMotor(-220, 220);
  }
  sensorRead();
 while(syc>2) {sensorRead();setMotor(-200, 200);}
  delay(50);
sensorRead();
	while (s_s5 != i) {
		s_s5 = digitalRead(s5);
		setMotor(-150, 150);
	} 
	delay(40);
	setMotor(0,0);delay(200);
	yon --;
	if (yon < 0) yon = 3;}
void setMotor(signed int sol, signed int sag){
	if (sol >= 0)
  {
    analogWrite(ENA, sol);
    digitalWrite(INA, 0);
  }
  else
  {
    analogWrite(INA, abs(sol));
    digitalWrite(ENA, 0);
  }
  if (sag >= 0)
  {
    analogWrite(ENB, sag);
    digitalWrite(INB, 0);
  }
  else
  {
    analogWrite(INB, abs(sag));
    digitalWrite(ENB, LOW);
  }
}
void sensorRead() {
  digitalWrite(led,i);
  hata1 = hata;
  hata = 0; syc = 0;

	s_s1 = digitalRead(s1); if (s_s1 == i) {hata = hata - 13;syc++;}
	s_s8 = digitalRead(s8); if (s_s8 == i) {hata = hata + 13;syc++;}
	s_s2 = digitalRead(s2); if (s_s2 == i) {hata = hata - 9;syc++;}
	s_s3 = digitalRead(s3); if (s_s3 == i) {hata = hata - 5; syc++;}
	s_s4 = digitalRead(s4); if (s_s4 == i) {hata = hata - 1; syc++;}
	s_s5 = digitalRead(s5); if (s_s5 == i) {hata = hata + 1;syc++;}
	s_s6 = digitalRead(s6); if (s_s6 == i) {hata = hata + 5;syc++;}
	s_s7 = digitalRead(s7); if (s_s7 == i) {hata = hata + 9;syc++;}
	if (syc > 0)hata = hata / syc;
	// else
	// {
	// if (hata1 >= 0)hata = 10;
	// if (hata1 < 0)hata = -10;
	// }
	delta_hata = hata - hata1;          //PID Controller
	P = hata * kp;
	D = delta_hata * kd;
	total = P + D;
	if (total > maxt) total = maxt;
	if (total < -maxt) total = -maxt;}

  
void cizgiTakip(){
	sensorRead();
	sol1 = (250 + total); sag1 = (250 - total);
	if (sag1 < -255) sag1 = -255; if (sag1 > 255) sag1 = 255;
	if (sol1 < -255) sol1 = -255; if (sol1 > 255) sol1 = 255;
	setMotor(sol1, sag1);}

  
void kareGec(){
    i=1;  sensorRead();while(syc<4) {cizgiTakip();}   setMotor(200,200); delay(200);//azGit(); // satir gir
    
   i=0;  sensorRead();  while(syc<4) {cizgiTakip();}  setMotor(200,200); delay(200);//setMotor(0,0);delay(2000);//azGit(); // satir ortayagel
     
   i=0;  sensorRead();  while(syc<4) {cizgiTakip();}  setMotor(200,200); delay(200);// azGit(); // satir cik
    neredeyim();
  
  }
void birakGel(){
  if(H_satir==4){
    i=1;  sensorRead();while(syc<4) {cizgiTakip();}   azGit(); // 4. satir
    yeniBirak();
    }
  else if(H_satir==3){
    kareGec();//4. geç
    i=1;  sensorRead();while(syc<4) {cizgiTakip();}   azGit(); // 3. satir gir
    yeniBirak();
    kareGec();//4. geç
  }
   else if(H_satir==2){
    kareGec();//4. geç
    kareGec();//3. geç
    i=1;  sensorRead();while(syc<4) {cizgiTakip();}   azGit(); // 2. satir gir
    yeniBirak();
    kareGec();//3. geç
    kareGec();//4. geç
    
    }
    else if(H_satir==1){
    kareGec();//4. geç
    kareGec();//3. geç
    kareGec();//2. geç
    i=1;  sensorRead();while(syc<4) {cizgiTakip();}   azGit(); // 2. satir gir
    yeniBirak();
    kareGec();//2. geç
    kareGec();//3. geç
    kareGec();//4. geç
    
    }
    
  i=1;  sensorRead();while(syc<4) {cizgiTakip();} azGit(); //5 satir 
  
  b_knt=1; // 5e geldik satir azaltma
buzOt(2);
  Serial1.println("5 geldik");
  //setMotor(0,0);while(1);
  if(cksGit==1) cikis();
 }
 
void yeniBirak(){
  servo.write(10); servo1.write(170);delay(600);
    setMotor(-170,-170);delay(500);
    Serial1.println("kutu birakildi");
    setMotor(0,0);delay(1000);
    solaDon();
    yon=2;
   // satir--;
   // neredeyim();
    //sensorRead();
   // while(syc<5) cizgiTakip();
   //azGit(); /// belki silmek gerekir
    //setMotor(0,0);
   // satir++;
  //  neredeyim();
    if(g_syc>kutuSayisi) cksGit=1;
    kutu_durum=0;
//    gorevAta(); 
//    G_sutun=H_sutun; 
//    H_sutun=sutun;
  }

  
void kutuAl(){
	setMotor(0,0);
	servo.write(130); servo1.write(50); delay(1000);}

  
void neredeyim(){
	Serial1.print("yon: ");
	Serial1.print(yon);
	Serial1.print(" Satir :");
	Serial1.print(satir);
	Serial1.print(" Sutun:");
	Serial1.println(sutun);}
  
void kavsak() { 
	if(b_knt==0){
//		if (yon == 0)      satir --;
//		else if (yon == 2) satir ++;
		if (yon == 1) sutun --;
		else if (yon == 3) sutun ++;
		neredeyim();
		azGit();
  }
	else b_knt=0;
	//-------------Sutun-İçin--------------------------
	//önce sutunu hizalamak için satırda hareket et
	if(sutun>H_sutun){                              
		while(yon!=1){
			if(yon==2) {solaDon();      }  
			else if(yon==3) {solaDon();solaDon(); }   
			else if(yon==0){ sagaDon();}
		}
  }                                                
	else if(sutun<H_sutun){
		while(yon!=3){
			if(yon==0) {solaDon(); } 
			else if(yon==1) {solaDon();solaDon();}   
			else if(yon==2) {sagaDon(); }
		}
  }
	
	else if(sutun==H_sutun){
      
      if(kutu_durum==0){
        while(yon!=2){
          if(yon==3) {solaDon();} 
          else if(yon==0) {solaDon();solaDon(); }   
          else if(yon==1) {sagaDon(); }
        }
        kutuAlGel();
        setMotor(0,0);
        Serial1.println("Birakma gorevi atayacak");
        gorevAta();
       
      }
      else{
        while(yon!=0){
          if(yon==1) {solaDon();} 
          else if(yon==2) {solaDon();solaDon(); }   
          else if(yon==3) {sagaDon(); }
        }
        birakGel();
        setMotor(0,0);
        Serial1.println("alma gorevi atayacak");
        gorevAta(); 
       b_knt=1;
        }
		}    
	}  
void bekle(int t){
  setMotor(0,0); delay(t);
  }
  
void azGeri(){
  //setMotor(-10,-10); delay(50);
  setMotor(-150,-150); delay(600);
  setMotor(0,0); delay(200);}
void azGit() {
  //setMotor(-20,-20); delay(30);
  setMotor(200,200); delay(200);
  setMotor(0,0); delay(100);}
  
void gorevAta()  {
 
	if(kutu_durum==0){
		H_satir=5; 
		H_sutun=al[g_syc]/10;
		H_Alt=al[g_syc]%10; 
		b_knt=0;	
	}
	else{
		H_satir=birak[g_syc]/10;
		H_sutun=birak[g_syc]%10; 
		g_syc++;
	}
	Serial1.print("Gorev No: ");
	Serial1.print(g_syc);
	
	if(kutu_durum==0) {
		Serial1.print(" Kutu Alma");
		Serial1.print("   H_sutun: ");
		Serial1.print(H_sutun);
		Serial1.print("   bolum: ");
		Serial1.println(H_Alt);
	}
	else         {
		Serial1.print(" Kutu Birakma");
		Serial1.print("   H_satir: ");
		Serial1.print(H_satir);
		Serial1.print("   H_sutun: ");
		Serial1.println(H_sutun);
	}}

  
void kutuAlGel() {
	sensorRead();while(syc<4)cizgiTakip();
	azGit(); // siyah bolume kadar git
	i=0;
	sensorRead();while(syc<4)cizgiTakip();
	azGit();
	if(H_Alt==1)
	{
		solaDon();
		sensorRead();  while(syc<4)cizgiTakip(); azGit();sagaDon();
		sensorRead();  while(syc<4)cizgiTakip();
   setMotor(150,150);delay(150);
		setMotor(0,0);delay(200);
		kutuAl();delay(500);
		azGeri();
		sagaDon();
		sensorRead();  while(syc<4)cizgiTakip();  azGit();sagaDon();
		sensorRead();  while(syc<4)cizgiTakip();  azGit();i=1;
	}
	else if(H_Alt==3)
	{
		sagaDon();  
		sensorRead();  while(syc<4)cizgiTakip();  azGit();solaDon();
		sensorRead();  while(syc<4)cizgiTakip();
    setMotor(150,150);delay(150);   
		setMotor(0,0);delay(200);
 
		kutuAl();delay(1000);
		azGeri();delay(1000);
		solaDon();
		sensorRead();  while(syc<4)cizgiTakip();  azGit();solaDon();
		sensorRead();  while(syc<4)cizgiTakip();  azGit();i=1;
	}
	else if(H_Alt==2)
	{
		sensorRead();
		while(syc<4)cizgiTakip();
   setMotor(150,150);delay(150);
		setMotor(0,0);delay(200);
  
		kutuAl();delay(1000);
		azGeri();delay(1000);
		//setMotor(0,0);while(1);
		solaDon();solaDon();delay(1000);
		sensorRead();  while(syc<4)cizgiTakip();  azGit();i=1;
	}
		sensorRead();
		while(syc<4)cizgiTakip();
		azGit(); yon=0;
		kutu_durum=1;
		b_knt=1;} // Malzeme aldıktan sonra 5. satıra yeniden gelidiği için satır azaltma
   
void giris(){ 
     i=0;
	 sensorRead();
	 while(syc<5) cizgiTakip();
	azGit();
	i=1;
	sensorRead();
	while(syc<4) cizgiTakip();
	azGit();
	sagaDon();}

 
void sutunGec(){
  i=1;
  sensorRead();
  while(syc<4) {cizgiTakip();} azGit();
  } 
void cikis(){
  i=1; 
  while(yon!=3){
      if(yon==0) {solaDon(); } 
      else if(yon==1) {solaDon();solaDon();}   
      else if(yon==2) {sagaDon(); }
    }
   if(sutun==1) {
      sutunGec(); // 2. sutun
      sutunGec(); // 3. sutun
      sutunGec(); // 4. sutun    
    }
    else if(sutun==2) {
      sutunGec(); // 3. sutun
      sutunGec(); // 4. sutun    
    } 
    else if(sutun==3) {
      sutunGec(); // 4. sutun    
    }

	sensorRead();
	while(syc<3) cizgiTakip();
	azGit();
	solaDon();
	sensorRead();
	while(syc<4) cizgiTakip();
	setMotor(200,200);
  delay(400);
	azGit();
  
	while(1);}
