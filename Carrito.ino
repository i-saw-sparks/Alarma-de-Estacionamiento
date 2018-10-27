#include <SPI.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include <TimerOne.h>
#include <Keypad.h>

#define Trig 2
#define Echo 3
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

char hexaKeys[4][4]=
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};

byte rowPins[4] = {47, 45, 43, 41};
byte colPins[4] = {39, 37, 35, 33};

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

Keypad Teclado = Keypad(makeKeymap(hexaKeys), rowPins, colPins, 4, 4);

const float VelSon = 34000.0;

int Contador=0;
float Distancia=26;
bool Status=false, Rev=false;
char Tecla='P';

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);

  TimerOne Timer1;
  tft.initR(INITR_BLACKTAB);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.fillScreen(ST77XX_BLACK);
}

void loop() {
  tft.fillScreen(ST77XX_BLACK);
  
  
    Tecla=Teclado.getKey();
    Serial.println(Tecla);
 

  if(Tecla=='*')
    Status=true;

  while(Rev==false&&Status==true){
    tft.setCursor(10,10);
    tft.print("D");
    Tecla=Teclado.getKey();
    if(Tecla=='1'){
      Rev=true;
      Timer1.initialize(100000);
      Timer1.attachInterrupt(Sonido);
      interrupts();
    }
    if(Tecla=='#')
      Status=false;
  }
  
  while(Rev==true&&Status==true){
  iniciarTrigger();
  unsigned long tiempo = pulseIn(Echo, HIGH);
  Distancia = tiempo * 0.000001 * VelSon / 2.0;
  
  Serial.print(Distancia);
  Serial.print("cm");
  Serial.println();
 
  if(Distancia >=10 && Distancia <15){
    tft.fillScreen(ST77XX_GREEN); 
    tft.setTextColor(ST77XX_BLACK);
  }
  else{
    if(Distancia >=5 && Distancia <10){
    tft.fillScreen(ST77XX_YELLOW);
    tft.setTextColor(ST77XX_BLACK); 
    }
    else{
       if(Distancia >=0 && Distancia <5){
        tft.fillScreen(ST77XX_RED);  
        tft.setTextColor(ST77XX_BLACK);
       }
        else{
          tft.fillScreen(ST77XX_BLACK); 
          tft.setTextColor(ST77XX_WHITE);
        }
     }
  } 
  tft.setCursor(10,10);
    tft.print("R");
  if(Distancia>2000){
    tft.setCursor(50,75);
    tft.println("OK");
  }else{
    tft.setCursor(0,75);
    tft.println("Obstaculo:");
    tft.print(Distancia);
    tft.println("cm");
  }
  Tecla=Teclado.getKey();
    if(Tecla=='2'){
      Rev=false;
      Timer1.stop();
    }
    if(Tecla=='#'){
      Status=false;
      Rev=false;
      Timer1.stop();
    }
  delay(500);
  }
}

void iniciarTrigger()
{
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
}

void Sonido(){
   if(Distancia >=25){
    if(Contador==12)
    tone(12,150);
    else
    noTone(12);
    if(Contador>12)
    Contador=0; 
   }
  if(Distancia >=20 && Distancia <25){
    if(Contador==6)
    tone(12,300);
    else
    noTone(12);
    if(Contador>6)
    Contador=0; 
  }
  if(Distancia >=15 && Distancia <20){
    if(Contador==4)
    tone(12,400);
    else
    noTone(12);
    if(Contador>4)
    Contador=0; 
  }
  if(Distancia >=10 && Distancia <15){
    if(Contador==3)
     tone(12,500);
     else
    noTone(12);
    if(Contador>3)
    Contador=0; 
  }
  if(Distancia >=5 && Distancia <10){
    if(Contador==2)
      tone(12,600);
    else
    noTone(12);
    if(Contador>1)
    Contador=0; 
  }
  if(Distancia >=0 && Distancia <5){
    if(Contador==1)
      tone(12,700);
    else
    noTone(12);
    if(Contador>1)
    Contador=0; 
  }
  Contador++; 
  
}
