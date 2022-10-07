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
// 'icons8-empty-battery-64', 13x13px
const unsigned char batteryBitmap [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x40, 0x20, 0x40, 0x30, 0x40, 0x30, 0x40, 0x30, 
  0x40, 0x20, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

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

//-------------UPDATE
//Update Display
void displayUpdate() {
  display.clearDisplay();
  
  //Draw images
  emptyShell();
  batteryIndicatorDraw();

  //Update display
  display.display();
}



#endif
