#include "Screen.h"
#include <TFT_eSPI.h>
#include <Simple_ACE.h>
#include "Calibration.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <EEPROM.h>
#include "ALE.h"

#include "Asset_2.h"
#include "Asset_7.h"
#include "Asset_8.h"
#include "Asset_10.h"
#include "Asset_13.h"
#include "Asset_14.h"
#include "setting.h"
#include "Beagle.h"
#include "BLE_Connect.h"


TFT_eSPI tft = TFT_eSPI();
TFT_eSprite graph1 = TFT_eSprite(&tft);
extern float ref_position[2];
float sample_1 = 0;
float sample_2 = 0;

int rangeL = 0;
int rangeH = 8000;
uint16_t beige =    tft.color565(239, 227, 214);
// uint32_t beige =    tft.color565(255, 244, 225);

float HighY = 100;
float LowY = 40;
uint16_t t_x = 0, t_y = 0;
int stage = 0;

void tft_setup(){
  tft.init();
  tft.fillScreen(TFT_NEIGHBOUR_GREEN);
  tft.setSwapBytes(true);
  
  graph1.setColorDepth(16);
  graph1.createSprite(200, 150);
  graph1.fillSprite(TFT_NEIGHBOUR_GREEN);
  graph1.setScrollRect(0, 0, 200,150, TFT_NEIGHBOUR_GREEN); 
}

void draw_BLE(){ /// doenst work and stalled the programm
  tft.pushImage(80, 50, BLE_w  ,BLE_h, BLE_Connect);
}

void ResetXY(){
  t_x = 0;
  t_y = 0;
}

void draw_framework(){
  tft.setTextColor(beige);
  tft.pushImage(5,200,A2_w,A2_h,Asset_2,0x0000);
  tft.pushImage(45,200,A7_w,A7_h,Asset_7,0x0000);
  tft.pushImage(85,200,A8_w,A8_h,Asset_8,0x0000);
  tft.pushImage(127,200,A10_w,A10_h,Asset_10,0x0000);
  tft.pushImage(167,200,A13_w,A13_h,Asset_13,0x0000);
  tft.pushImage(206,200,A14_w,A14_h,Asset_14,0x0000);
  tft.setTextDatum(TC_DATUM); 
  tft.drawString("Acetone Level",120, 10,4);
  tft.setTextDatum(TL_DATUM); 
  // tft.drawString("King's ",8, 250,2);
  // tft.drawString("Phase",8,270,2);
  // tft.drawString("Technologies",8,290,2);
  tft.pushImage(180, 260, settingWidth  ,settingHeight, setting);
}

void draw_time(int time){
    tft.fillRect(45,200,148,50,TFT_NEIGHBOUR_GREEN); 
    tft.setTextColor(beige);
    tft.setTextDatum(TC_DATUM); 
    tft.drawFloat(time, 0, 120, 200, 6);
}
void draw_wait(void){  
  tft.fillRect(70,210,110,50,TFT_NEIGHBOUR_GREEN);
  tft.setTextDatum(TC_DATUM); 
  tft.setTextColor(beige);
  tft.drawString("Analyzing...",120, 250,4);

}

void set_range(int value){
  rangeL = value -1000;
  rangeH = (value +3000*1.3); 
  tft.setTextDatum(4);
  tft.setTextColor(TFT_NEIGHBOUR_BEIGE,TFT_NEIGHBOUR_GREEN);
    tft.fillRect(10,120,240,30,TFT_NEIGHBOUR_GREEN);
  tft.fillRect(0,40,240,80, TFT_NEIGHBOUR_GREEN);
  tft.drawString("HUFF now",120, 250, 4);
}

void draw_sensor(double value){
    graph1.scroll(-1); 
    // Move sprite content 1 pixel left. Default dy is 0
    // value = map(value,rangeL,rangeH,0,100);
    value =  (value-rangeL)*(150*3.14-0)/(rangeH-rangeL)+0;
    // printf("%f\n",value); 
    graph1.drawFastVLine(199, 150 - (int)(150*(sin((value/150.0)-(3.14/2.0))+1.0)),5, beige);
    graph1.pushSprite(20, 40);
    // graph1.drawString("Breathe Here, ")
}
// bool store;
extern int fail_count;
void draw_result(double ace, double co2){
  tft.fillRect(70,200,100,50,TFT_NEIGHBOUR_GREEN); 
  tft.fillRect(50,250,140,30,TFT_NEIGHBOUR_GREEN); 
  tft.pushImage(45,200,A7_w,A7_h,Asset_7,0x0000);
  tft.pushImage(85,200,A8_w,A8_h,Asset_8,0x0000);
  tft.pushImage(127,200,A10_w,A10_h,Asset_10,0x0000);
  tft.pushImage(167,200,A13_w,A13_h,Asset_13,0x0000);  

  tft.setTextDatum(4); 
  tft.fillRoundRect(8, 270, 60, 46,23 ,TFT_NEIGHBOUR_BLUE);
  tft.drawRoundRect(8, 270, 60, 46,23 ,TFT_WHITE);
  tft.setTextColor(TFT_WHITE, TFT_NEIGHBOUR_BLUE);
  tft.drawString("Start", 38 ,293,2);        
  if(fail_count != 50){
    tft.setTextColor(TFT_WHITE, TFT_NEIGHBOUR_GREEN);
    tft.drawString("Acetone",180, 125,2);
    tft.drawFloat((float)ace,2,180,140,2);
    tft.drawString("Metabolic rate", 60, 125,2);
    tft.drawFloat((float)co2*100,2,60,140,2);
  }
  tft.setTextColor(TFT_WHITE, TFT_NEIGHBOUR_GREEN);
  if(ace > 1 || ace <= 0||store == false){
  tft.drawString("Try Again",120,60,4); 
  }
  else if (ace > 0.96 && ace <= 1){
  tft.drawString("Workout More!",120,60,4);
  }
  else if(ace > 0.9 && ace <= 0.96){
  tft.drawString("Pretty Good!",120,60,4);
  }
  else if(ace <= 0.9 && ace > 0){
    tft.drawString("Excellent Fat Burn!",120,60,4);
  } 
}

  void HomeScreen(){
    tft.pushImage(20,80,BeagleWidth, BeagleHeight, Beagle);
    tft.setTextDatum(0);
    tft.setTextColor(TFT_WHITE, TFT_NEIGHBOUR_GREEN);
    tft.drawString("King's ",1, 250,2);
    tft.drawString("Technologies",1,270,1);
    tft.drawString("Phase",1,280,2);

    tft.pushImage(5,200,A2_w,A2_h,Asset_2,0x0000);
    tft.pushImage(45,200,A7_w,A7_h,Asset_7,0x0000);
    tft.pushImage(85,200,A8_w,A8_h,Asset_8,0x0000);
    tft.pushImage(127,200,A10_w,A10_h,Asset_10,0x0000);
    tft.pushImage(167,200,A13_w,A13_h,Asset_13,0x0000);
    tft.pushImage(206,200,A14_w,A14_h,Asset_14,0x0000);
    
    tft.pushImage(180, 260, settingWidth  ,settingHeight, setting);

  }

void TouchScreen(){
  if(stage == 0){
    // tft.fillScreen(TFT_NEIGHBOUR_GREEN);
    HomeScreen();
    // tft.drawString("Beagle",100, 60 , 4);
  }
  if(tft.getTouch(&t_x, &t_y)){
    printf("%d\n", t_x);
    printf("%d\n", t_y);
    if(stage == 0 || stage ==2 || stage ==3 || stage ==4){
      if(t_x > 0 && t_x < 35  && t_y > 245 && t_y < 290){
        tft.fillScreen(TFT_NEIGHBOUR_GREEN); 
        tft.setTextColor(TFT_NEIGHBOUR_BLUE, TFT_NEIGHBOUR_BEIGE);
        tft.setTextDatum(4);
        
        tft.fillRoundRect(10, 10, 220, 60,30 ,TFT_NEIGHBOUR_BEIGE);
        tft.drawRoundRect(10, 10, 220, 60,30, TFT_NEIGHBOUR_BLUE);
        tft.drawString("OTA Setting",120,40,4);

        tft.fillRoundRect(10, 90, 220, 60,30 , TFT_NEIGHBOUR_BEIGE);
        tft.drawRoundRect(10, 90, 220, 60, 30,TFT_NEIGHBOUR_BLUE);
        tft.drawString("Calibration",120,120,4);
        
        tft.fillRoundRect(10, 170, 220, 60,30 ,TFT_NEIGHBOUR_BEIGE);
        tft.drawRoundRect(10, 170, 220, 60,30, TFT_NEIGHBOUR_BLUE);
        tft.drawString("Sampling",120,200,4);

        tft.fillRoundRect(10, 250, 220, 60,30 ,TFT_NEIGHBOUR_BEIGE);
        tft.drawRoundRect(10, 250, 220, 60,30, TFT_NEIGHBOUR_BLUE);
        tft.drawString("Return",120,280,4);

        stage= 1;
        ResetXY();
        delay(400);
      }
    }

    if(stage == 1){ // Navigation
      if(t_x > 10 && t_x < 50  && t_y >20  && t_y < 290){              //return button
        // DrawHomescreen();
        stage = 0;
        sample_1+=5;
        tft.fillScreen(TFT_NEIGHBOUR_GREEN);
        ALE_notify();
        ResetXY();
      }
      else if(t_x > 75 && t_x < 115  && t_y >20  && t_y < 290){ //PID_controller
        ResetXY();
        tft.fillScreen(TFT_NEIGHBOUR_GREEN);
        draw_framework();
        tft.fillRoundRect(8, 270, 60, 46,23 ,TFT_NEIGHBOUR_BLUE);
        tft.drawRoundRect(8, 270, 60, 46,23 ,TFT_WHITE);
        tft.setTextColor(TFT_WHITE, TFT_NEIGHBOUR_BLUE);

        tft.drawString("Start", 20,285,2);

        // tft.fillRoundRect(90, 270, 60, 46,23 ,TFT_NEIGHBOUR_BLUE);
        // tft.drawRoundRect(90, 270, 60, 46,23 ,TFT_WHITE);
        // tft.drawString("END", 110,285,2);
        //drawKeypad();

        stage = 2;
        printf("stage2 \n");
      } 
      else if(t_x > 140 && t_x < 180  && t_y >20  && t_y < 290){ //Collaboration
        tft.fillScreen(TFT_NEIGHBOUR_GREEN);
        tft.pushImage(180, 260, settingWidth  ,settingHeight, setting);
        tft.fillRoundRect(10, 10, 220, 30,15 ,TFT_NEIGHBOUR_BLUE);
        tft.setTextColor(TFT_WHITE, TFT_NEIGHBOUR_BLUE);
        tft.drawString("Calibration", 120,25,2);
        printf("stage3 \n");

        stage = 3;
        ResetXY();
        tft.fillRect(10,250,80,40,TFT_NEIGHBOUR_BLUE);     //Start Button
        tft.drawString("START", 50,270,2);
      }
      else if (t_x > 200 && t_x < 240  && t_y >20  && t_y < 290){ //OTA Setting
        ResetXY();
        printf("stage4 \n");
        tft.fillScreen(TFT_NEIGHBOUR_GREEN);
        tft.pushImage(180, 260, settingWidth  ,settingHeight, setting);
        tft.fillRoundRect(10, 10, 220, 30,15 ,TFT_NEIGHBOUR_BLUE);
        tft.setTextColor(TFT_WHITE, TFT_NEIGHBOUR_BLUE);
        tft.setTextDatum(4);
        tft.drawString("BLUETOOTH Setting", 120,25,2);

        tft.fillRect(10,250,80,40,TFT_NEIGHBOUR_BLUE);
        tft.setTextColor(TFT_WHITE,TFT_NEIGHBOUR_BLUE);     //Start Button
        tft.drawString("Advertise", 50,270,2);
        
        
        stage = 4;
      } 
    }

    if(stage == 2){
      int Open = 1;
      if(t_x > 11 && t_x < 32  && t_y > 15 && t_y < 75){
        Open = 1;
        if(Open = 1){
          sample_collection();
          output_result();
        }
        else if(t_x > 5 && t_x < 27  && t_y > 110 && t_y < 196){
          Open = 0;
        }  
      }
    }

    if(stage == 3){                                           //Calibration Start Button
      if(t_x > 22 && t_x < 47  && t_y >13  && t_y < 108){
        tft.setTextDatum(4);
        long previous = 0;
        int i = 3;
        while(i >= 0){
          if(millis()- previous> 1000){
            previous = millis();
            tft.fillRect(10,80,200,150,TFT_NEIGHBOUR_GREEN);
            tft.setTextColor(TFT_WHITE, TFT_NEIGHBOUR_GREEN);
            tft.drawFloat((float) i, 0,120,160,6);
            i--;
          }
        }

        tft.fillRect(10,80,200,150,TFT_NEIGHBOUR_GREEN);
        tft.setTextColor(TFT_WHITE, TFT_NEIGHBOUR_GREEN);
        tft.drawString("Calibrating",120,160,4);

        calibration();
        tft.fillRect(10,80,200,150,TFT_NEIGHBOUR_GREEN);
        printf("Screen Display.");

        EEPROM.begin(20);
        int value,value_1;
        byte address = 0;
        EEPROM.get(address,value);
        delay(100);
        tft.drawFloat(float(value),0, 80,120, 2);
        address += sizeof(int);
        EEPROM.get(address,value_1);
        delay(100);
        tft.drawFloat(float(value_1),0, 160,120,2);
        EEPROM.end();
        delay(500);
      } 
    }
    if(stage ==4){
      if(t_x > 22 && t_x < 47  && t_y >13  && t_y < 108){
        ALE_advertise();
      }
    }
  }
}

