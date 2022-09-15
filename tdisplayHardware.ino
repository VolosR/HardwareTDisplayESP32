#include <TFT_eSPI.h> //Select #include <User_Setups/Setup135_ST7789.h> // Setup file for ESP8266 and ST7789 135 x 240 TFT
#include "fonts.h"

#define BTN 0




TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);


String dd[15];
String ddT[15];
int first=0;
int counter=0;
int fase=0;
bool recived=false;
int brightness[5]={30,60,120,180,250};
int curB=1; //current brightness
int deb=0;

long timeStart=0;
long timeEnd=0;
float fps=0;

const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;

void setup()
{

 

 
  

  Serial.begin(115200);

  tft.init();
 
  
  tft.setSwapBytes(true);

tft.fillScreen(TFT_BLACK);
sprite.createSprite(135,240);
sprite.setTextDatum(4);

  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(5, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, brightness[curB]);
 
 
 
  
}
int i=0;

//position variables
int cpuY=64;
int cpuY2=cpuY+96;
int boxW=60;
int boxH=70;
int x1=4;
int x2=68;
void loop()
{
timeStart=millis();

if(digitalRead(BTN)==0)
{
  if(deb==0)
  {
    deb=1;
    curB++;
    if(curB==5) curB=0;
    ledcWrite(pwmLedChannelTFT, brightness[curB]);
    }
  }else deb=0;
  
sprite.fillSprite(TFT_BLACK);  

for(int i=0;i<curB+1;i++)
sprite.fillRect(112+(i*4),4,2,8,TFT_ORANGE);

sprite.drawRoundRect(x1,cpuY,boxW,boxH,4,TFT_WHITE);
sprite.drawRoundRect(x2,cpuY,boxW,boxH,4,TFT_WHITE);

sprite.drawRoundRect(x1,cpuY2,boxW,boxH,4,TFT_WHITE);
sprite.drawRoundRect(x2,cpuY2,boxW,boxH,4,TFT_WHITE);

sprite.setTextColor(0x9CF3,TFT_BLACK);
sprite.drawString("CPU",x1+30,cpuY+12,2);
sprite.drawString("POWER",x2+30,cpuY+12,2);
sprite.drawString("RAM",x1+30,cpuY2+12,2);
sprite.drawString("HDD",x2+30,cpuY2+12,2);

sprite.drawString("%",x1+30,cpuY+57,2);
sprite.drawString("W",x2+30,cpuY+57,2);
sprite.drawString("%",x1+30,cpuY2+57,2);
sprite.drawString("%",x2+30,cpuY2+57,2);
sprite.setTextColor(TFT_ORANGE,TFT_BLACK);
sprite.drawString("Memory",32,cpuY+83,2);
sprite.drawString("Processor",38,cpuY-12,2);
sprite.drawString("FPS:"+String((int)fps),106,236);

while (Serial.available() > 0)
   { 
    recived=true;
    char text = Serial.read();
    
    if(text=='"')
    fase++;
    if(fase==2){
    fase=0;
    counter++;
    }
    if(fase==1)
    dd[counter]=dd[counter]+String(text);
    }

  if(counter>5){
    
  for(int i=0;i<counter;i++){
    ddT[i]=dd[i];
    dd[i]="";
    
  //sprite.drawString("      "+String(ddT[i])+"     ",10,i*18,2);
  }
  counter=0;}

  if(ddT[5].substring(1,ddT[5].length()-1)=="100")
  
  sprite.fillCircle(14,12,6,TFT_RED);
 
  
   sprite.setFreeFont(&Lato_Hairline_24);
    sprite.setTextColor(TFT_WHITE,TFT_BLACK);
    sprite.drawString(ddT[3].substring(1,ddT[3].length()),67,10); //draw time
    sprite.drawString(ddT[4].substring(5,ddT[4].length()),67,34,2); //draw DATE
     sprite.setFreeFont(&Roboto_Mono_Light_26);
     
    sprite.drawString(ddT[5].substring(1,ddT[5].length()-1),x1+30,cpuY+34); //draw cpu
    sprite.drawString(ddT[0].substring(1,ddT[0].length()-2),x2+30,cpuY+34); //draw cpuPower
        sprite.drawString(ddT[1].substring(1,ddT[1].length()-1),x1+30,cpuY2+34); //draw RAM
            sprite.drawString(ddT[2].substring(1,ddT[2].length()-1),x2+30,cpuY2+34); //draw cpu HDD
            sprite.setTextFont(0);


 
            
   sprite.pushSprite(0,0);
   timeEnd=millis();
   fps=1000/(timeEnd-timeStart); 
}
