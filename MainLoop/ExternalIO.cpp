//This file is namely for external inputs like buttons NOT I2S devices like the mic and speaker
#include "ExternalIO.h"

//Pin declare
byte batteryMonitorPin = 4;
byte debugPushButtonPin = 5;
byte upButtonPin = 5;
byte downButtonPin = 19;
byte rightButtonPin = NULL;
byte leftButtonPin = NULL;

//timing
const int longPressThreshhold = 250; //ms
int prevMillisUp = 0;
int prevMillisDown = 0; 

//states
bool isUpButtonPushed = false;
bool isDownButtonPushed = false;

typedef struct interfaceButtons { 
  //Pin Declares
  byte buttonPin;

  //Press statuses
  char* pressStatus;

  //Same press detection
  bool samePress;

  //Button States
  int previousState;
  int currentState;

  //Time stamps
  unsigned long timePressed;
  unsigned long timeReleased;
};

void inputsInitialize() {
  pinMode(batteryMonitorPin, INPUT);
  pinMode(debugPushButtonPin, INPUT_PULLUP);
  pinMode(upButtonPin, INPUT_PULLUP);
  pinMode(downButtonPin, INPUT_PULLUP);

  /*  0: up
      1: down
      2: right
      3: left
  */
  interfaceButtons buttons[4];
  buttons[0].buttonPin = 5;
  buttons[1].buttonPin = 19;
  buttons[2].buttonPin = NULL;;
  buttons[3].buttonPin = NULL;
}

unsigned short int pollBattery() {
  unsigned short int batteryLevelRaw;
  batteryLevelRaw = analogRead(batteryMonitorPin);

  return batteryLevelRaw;
}

void pushButtonState(byte pushButtonPin, char* pressStatus, bool samePress, int previousState, int currentState, unsigned long timePressed, unsigned long timeReleased) {
  unsigned long pressDuration = 0;

  // 0 is unpressed, 1 is pressed
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
    //If function hasn't initialized
    previousState = LOW;
    samePress = false;
  }
}

bool rawPushButton() {
  return digitalRead(debugPushButtonPin);
}

void ioUpdate() {
  for (int i = 0; i < 3, i++) { 
    pushButtonState(buttons[i]].buttonPin, buttons[i].pressStatus, buttons[i].samePress, buttons[i].previousState, buttons[i].currentState, buttons[i].timePressed, buttons[i].timeReleased);
  }
}
