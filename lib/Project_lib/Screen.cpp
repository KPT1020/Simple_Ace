#include "Screen.h"
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite graph1 = TFT_eSprite(&tft);

int rangeL = 2000;
int rangeH = 18000;
uint16_t green =    tft.color565(23, 92, 66);
// uint32_t beige =    tft.color565(255, 244, 225);


void tft_setup(){
  tft.init();
  tft.fillScreen(TFT_RED);
  graph1.setColorDepth(16);
  graph1.createSprite(200, 150);
  graph1.fillSprite(TFT_RED);
}

void draw_framework(){
  tft.drawString("Acetone Level",1, 5,2);
  tft.drawString("10000",1, 32);
  tft.drawString("2000",10, 132);
  tft.drawString("King's ",1, 250,2);
  tft.drawString("Technologies",1,270,1);
  tft.drawString("Phase",1,280,2);
}

void draw_humid(double H){
    tft.setTextColor(TFT_NEIGHBOUR_GREEN);
    tft.drawFloat(float(H), 1, 70, 180, 6);
}

void draw_sensor(double value){
    graph1.scroll(-1); 
    printf("%d\n",value);
    // Move sprite content 1 pixel left. Default dy is 0
    // value = map(value,rangeL,rangeH,0,100);
    value =  (value-rangeL)*(150*3.14-0)/(rangeH-rangeL)+0;
    printf("%lf\n",value);
    graph1.drawFastVLine(199, 150 - 150*(sin((value/150)-(3.14/2))+1),3, TFT_NEIGHBOUR_GREEN);
    graph1.pushSprite(20, 40);
}

void draw_result(double value){
  tft.drawString("Acetone",180, 270,2);
  tft.drawFloat((float)value,2,180, 290,1);
  delay(1000);
  tft.fillRect(270,170,40,30,TFT_WHITE);
}