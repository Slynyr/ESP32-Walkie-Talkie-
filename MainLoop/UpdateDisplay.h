#ifndef UPDATE_DISPLAY_H
#define UPDATE_DISPLAY_H
#include <Arduino.h>

//Display Definitions
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

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

void batteryIndicator() {
  display.drawBitmap(2, 1, batteryBitmap, 13, 13, WHITE);
  display.drawPixel(4,5, WHITE);
  display.drawPixel(11,5, WHITE);
  display.drawPixel(4,15, WHITE);
}

//-------------UPDATE
//Update Display
void displayUpdate() {
  
  //Draw images
  emptyShell();
  batteryIndicator();

  //testing text function
  drawText(0,0, 2, "This is a test");

  //Update display
  display.display();
}



#endif
