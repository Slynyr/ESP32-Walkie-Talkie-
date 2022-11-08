//This file is namely for external inputs like buttons NOT I2S devices like the mic and speaker
#include "ExternalIO.h"

//Pin declare
byte batteryMonitorPin = 4;
byte debugPushButtonPin = 5;
byte upButtonPin = 5;
byte downButtonPin = 19;

//button press types
 char* upPressStatus;

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

void pushButtonState(byte pushButtonPin, char* pressStatus) {
  // 0 is unpressed, 1 is pressed
  int previousState;
  int currentState;
  // Won't update properly due to being a local variable
  bool samePress;

  unsigned long timePressed = 0;
  unsigned long timeReleased = 0;
  unsigned long pressDuration = 0;

  currentState = digitalRead(pushButtonPin);
  if (previousState == LOW && currentState == HIGH) {  //Button pressed
    if (samePress == false) {
      timePressed = millis(); //Log pressed time
    }
    samePress = true;
    previousState = HIGH;

    if (millis() - timePressed >= SHORT_PRESS_TIME) {
      pressStatus = "LONG";
    }

  } else if (previousState == HIGH && currentState == LOW) { //Button released
    timeReleased = millis(); //Log released time
    previousState = LOW;
    samePress = false;

    pressDuration = timeReleased - timePressed; //Pressed duration is these times subtracted

    if (pressDuration <= SHORT_PRESS_TIME) {
      pressStatus = "SHORT";
    }

  } else {
    //Function hasn't initialized
    previousState = LOW;
    samePress = false;
  }
}

bool rawPushButton() {
  return digitalRead(debugPushButtonPin);
}

void ioUpdate() {
  pushButtonState(upButtonPin, upPressStatus);
}
