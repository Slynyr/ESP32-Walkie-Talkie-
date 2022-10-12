//This file is namely for external inputs like buttons NOT I2S devices like the mic and speaker
#ifndef EXTERNAL_IO_H
#define EXTERNAL_IO_H
#include <Arduino.h>

//Pin declare
byte batteryMonitorPin = 4;
byte debugPushButtonPin = 5; //CHANGE

bool isDebugTriggered = false;

//---------------INIT
void inputsInitialize() {
  pinMode(batteryMonitorPin, INPUT);
  pinMode(debugPushButtonPin, INPUT);
}

unsigned short int pollBattery() {
  unsigned short int batteryLevelRaw;
  batteryLevelRaw = analogRead(batteryMonitorPin);
  
  return batteryLevelRaw;
}

//void debugPushbutton(){
//  if (digitalRead(debugPushButtonPin) == HIGH){
//    isDebugTriggered = true;
//  } else {
//    isDebugTriggered = false;
//  }

void updateButtons(){
  isDebugTriggered = digitalRead(debugPushButtonPin);
}
#endif
