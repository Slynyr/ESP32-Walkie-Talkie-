//This file is namely for external inputs like buttons NOT I2S devices like the mic and speaker
#include "ExternalIO.h"

//Pin declare
byte batteryMonitorPin = 4;
byte debugPushButtonPin = 5;

void inputsInitialize() {
  pinMode(batteryMonitorPin, INPUT);
  pinMode(debugPushButtonPin, INPUT_PULLUP);
}

unsigned short int pollBattery() {
  unsigned short int batteryLevelRaw;
  batteryLevelRaw = analogRead(batteryMonitorPin);

  return batteryLevelRaw;
}

bool debugPushbutton() {
  return digitalRead(debugPushButtonPin);
}