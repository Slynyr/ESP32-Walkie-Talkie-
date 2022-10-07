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
void drawText(int cursX, int cursY, int textSize, char * message){
    display.setTextSize(textSize);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(message);
}

//--------------Draw Types
void emptyShell() {
  //Frame of screen boundaries
  display.drawRect(0, 0, 128, 15, WHITE);
  display.drawRect(0, 16, 128, 48, WHITE);
}

void batteryIndicatorValues(unsigned short int batteryLevelRaw) {
  //Voltage and level math  //3310-> ~3.5V, 4095-> ~4.2V
  batteryLevel = map(batteryLevelRaw, 3310, 4095, 0, 8);
  voltageLevel = ((batteryLevelRaw * 4.2) / 4095);
}

void batteryIndicatorDraw() {
  //Battery internal size is 8x4
  display.drawBitmap(2, 1, batteryBitmap, 13, 13, WHITE);

  //Draw rectangle based on battery level
  display.fillRect(4, 4, batteryLevel, 6, WHITE);
}

//--------Node Status Icon 
void nodeConnectionStatus(bool isConnected, int posX, int posY){
  if (isConnected){
    display.drawBitmap(posX,posY, nodeConnectionEstablished, 13, 13, WHITE);
  } else {
    display.drawBitmap(posX,posY, noNodeConnectionmap, 13, 13, WHITE);
  }
}

//-------------UPDATE
//Update Display
void displayUpdate() {
  display.clearDisplay();
  
  //Draw images
  emptyShell();
  batteryIndicatorDraw();

  //testing functions
    //drawText(0,0, 1, "This is a test");
  nodeConnectionStatus(true, 100, 1);

  //Update display
  display.display();
}



#endif
