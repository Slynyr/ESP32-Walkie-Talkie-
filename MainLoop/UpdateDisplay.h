//This file is for display drawing and screen updating only
#ifndef UPDATE_DISPLAY_H
#define UPDATE_DISPLAY_H
#include <Arduino.h>
#include "ExternalIO.h"

//Display Definitions
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

//Globals
float voltageLevel;
unsigned short int batteryLevel;
char * state = "main";

//---------------INIT
void displayInitialize() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();
}

//---------------ICONS
// 'Battery Icon', 13x13px
const unsigned char batteryBitmap [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x40, 0x20, 0x40, 0x30, 0x40, 0x30, 0x40, 0x30, 
  0x40, 0x20, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 'Node Connection Established', 13x13px
const unsigned char nodeConnectionEstablished [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x7f, 0xf0, 0xf0, 0x78, 0x4f, 0x90, 0x1f, 0xc0, 0x18, 0xc0, 
	0x07, 0x00, 0x07, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00
};

// 'Node Connection Failed Icon', 13x13px
const unsigned char noNodeConnectionmap [] PROGMEM = {
	0x00, 0x00, 0x02, 0x00, 0x02, 0x00, 0x3f, 0xe0, 0x7a, 0xf0, 0xe2, 0x38, 0x1a, 0xc0, 0x18, 0xc0, 
	0x02, 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00
};


//------------DRAWTEXTFUNCTION
void drawText(bool isCenter, int cursX, int cursY, int textSize, char * message){
    display.setTextSize(textSize);
    display.setTextColor(WHITE);
    display.setTextWrap(false);
    if (isCenter){
      int16_t x1, y1;
      uint16_t w, h;    
      display.getTextBounds(message, cursX, cursY, &x1, &y1, &w, &h); //calc width of new string
      display.setCursor(cursX - w / 2, cursY); 
      display.print(message);
    } else {
      display.setCursor(cursX,cursY);
      display.print(message);
    }
    display.setTextWrap(true);
}

void drawCenteredCircle(bool isFilled, int cursX, int cursY, int size){
  if (isFilled){
    display.fillCircle(cursX - (size/2), cursY - (size/2), size, WHITE);
  } else {
    display.drawCircle(cursX - (size/2), cursY - (size/2), size, WHITE);
  }
}

//--------------DRAW-TYPES
void backdrop(const int mode) {
  //Frame of screen boundaries
  if (mode == 1){
    display.drawRect(0, 0, 128, 15, WHITE);
    display.drawRect(0, 16, 128, 48, WHITE);  
  } else if (mode == 2){
    display.drawLine(0, 15, 128, 15, WHITE);
  } else if (mode == 3) {
    display.drawLine(0, 15, 128, 15, WHITE);
    display.drawLine(0, 16, 128, 16, WHITE);
  }
}

void batteryIndicatorValues(unsigned short int batteryLevelRaw) {
  //Voltage and level math 
  //3310-> ~3.5V, 4095-> ~4.2V Using R1 5k1 R2 20k - Better for low voltage accuracy
  //3090-> ~3.5V, 4095-> ~4.2V Using R1 47K R2 150K - Better for higher voltage accuracy
  batteryLevel = map(batteryLevelRaw, 3310, 4095, 0, 8);
  voltageLevel = ((batteryLevelRaw * 4.2) / 4095);
}

void batteryIndicatorDraw() {
  //Battery internal size is 8x4
  display.drawBitmap(2, 1, batteryBitmap, 13, 13, WHITE);

  //Draw rectangle based on battery level
  display.fillRect(4, 4, batteryLevel, 6, WHITE);

  //Draw text for voltage
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17, 4);
  display.print("~");
  display.print(voltageLevel);
  display.print("V");
}

//--------Mode Connection Status Icon 
void modeConnectionStatus(char * mode, bool isConnected, int posX, int posY) {
  if (mode == "NODE"){
     if (isConnected){
      display.drawBitmap(posX,posY, nodeConnectionEstablished, 13, 13, WHITE);
    } else {
      display.drawBitmap(posX,posY, noNodeConnectionmap, 13, 13, WHITE);
    } 
  } else if (mode == "LOCAL") {
    ;
    //render local mode sprites
  }
}


//--------Lower Screen Menus 
void drawIntWithSubheading(bool isConnected, int posX, int posY, int value, char * subHeadint){
  if (isConnected){
    char msgBuffer[32] = {0};
    sprintf(msgBuffer, "%d", value);

    drawText(true, posX, posY, 3.8, msgBuffer);
    drawText(true, posX, posY + 23, 1.5, subHeadint);
  } else {
    drawText(true, posX, posY, 3.5, "NO CON");
  }
}

void lowerScreenMain(int channelCount, int userCount){
  drawIntWithSubheading(true, 26, 25, channelCount, "Channel"); //Channe counter
  drawIntWithSubheading(true, 102, 25, userCount, "Users"); //User counter
}

//-----------Dynamic Animations
void nodeAnimation(int clientsConnected, bool isMaster){
  //Not important just ignore this
  if (clientsConnected >= 2){
    //center
    display.drawCircle(20, 20, 10, WHITE);
  }
}

//-------------UPDATE
//Update Display
void displayUpdate() {
  display.clearDisplay();

  if (state == "main") {  
    //Draw images
    backdrop(2);
    batteryIndicatorDraw();
    
    drawText(true,64, 32, 1, "HI");
    modeConnectionStatus("NODE", false, 100, 1);
    lowerScreenMain(24, 12);

    //Update display
  } else if (state == "menu") {
    ;//menu selection screen here
  } 

  display.display();
}

#endif
