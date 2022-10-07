//This file is namely for external inputs like buttons NOT I2S devices like the mic and speaker
#ifndef EXTERNAL_IO_H
#define EXTERNAL_IO_H
#include <Arduino.h>

//Pin declare
byte batteryMonitorPin = 4;

//---------------INIT
void inputsInitialize() {
  pinMode(batteryMonitorPin, INPUT);
}

unsigned short int pollBattery() {
  unsigned short int batteryLevelRaw;
  batteryLevelRaw = analogRead(batteryMonitorPin);
  
  return batteryLevelRaw;
}
#endif
