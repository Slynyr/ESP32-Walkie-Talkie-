//This file is for display drawing and screen updating only
//Used to generate bitmaps: https://javl.github.io/image2cpp/ 
#ifndef _UPDATE_DISPLAY_H_
#define _UPDATE_DISPLAY_H_

//Display Definitions
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Arduino.h>

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define OLED_ADDR 0x3C

void displayInitialize();
  //Initialize display variables and library

void getDisplayMillis(unsigned long masterMillis);
  //Get millis from main

void drawText(bool isCenter, int cursX, int cursY, int textSize, char *message);
  //Function to accelerate drawing text

void backdrop(const int mode);
  //Display frame type

void batteryIndicatorValues();
  //Voltage calculations for displaying battery status

void batteryWarnToggle();
  //Warns when the battery is below a threshold voltage

void batteryIndicatorDraw(const int colour);
  //Draws the battery indicator

void modeConnectionStatus(char *mode, bool isConnected, int posX, int posY);
  //Sets and draws the connection status of the ESP

void drawIntWithSubheading(bool isConnected, int posX, int posY, int value, char *subHeadint);
  //???

void lowerScreenMain(int channelCountValue, int userCountValue);
  //Draws channel and usercount

void nodeAnimation(int clientsConnected, bool isMaster);
  //WIP

void displayUpdate();
  //Update display with drawn objects

#endif