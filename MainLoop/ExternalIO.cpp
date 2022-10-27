//This file is namely for external inputs like buttons NOT I2S devices like the mic and speaker
#include "ExternalIO.h"

//Pin declare
byte batteryMonitorPin = 4;
byte debugPushButtonPin = 5;
byte upButtonPin = 5;
byte downButtonPin = 19;

//timing
const int longPressThreshhold = 250; //ms
int prevMillisUp = 0;
int prevMillisDown = 0; 

//states
bool isUpButtonPushed = false;
bool isDownButtonPushed = false;

void inputsInitialize() {
  pinMode(batteryMonitorPin, INPUT);
  pinMode(debugPushButtonPin, INPUT_PULLUP);
  pinMode(upButtonPin, INPUT_PULLUP);
  pinMode(downButtonPin, INPUT_PULLUP);
}

unsigned short int pollBattery() {
  unsigned short int batteryLevelRaw;
  batteryLevelRaw = analogRead(batteryMonitorPin);

  return batteryLevelRaw;
}

bool debugPushbutton() {
  return digitalRead(debugPushButtonPin);
}

char* upButton(){
  if (digitalRead(upButtonPin) && !isUpButtonPushed){
    isUpButtonPushed = true;
    prevMillisUp = millis(); //ik colin Ill chabge it 
  } else if (!digitalRead(upButtonPin) && isUpButtonPushed){
    if ((millis() - prevMillisUp) >= longPressThreshhold){
      isUpButtonPushed = false;
      Serial.println(millis() - prevMillisUp);
      return "LP";
    } else{
      isUpButtonPushed = false;
      Serial.println("SP TRIGGERED");
      Serial.println(millis() - prevMillisUp);
      return "SP";
    }
  }
  return "NULL";
}