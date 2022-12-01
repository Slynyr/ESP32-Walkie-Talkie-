//This file is namely for external inputs like buttons NOT I2S devices like the mic and speaker
#include "ExternalIO.h"


//Pin declare
byte batteryMonitorPin = 35;
byte debugPushButtonPin = 5;
byte upButtonPin = 5;
byte downButtonPin = 19;
byte rightButtonPin = NULL;
byte leftButtonPin = NULL;

//Initialize structure
interfaceButtons buttons[4];

//timing
const int longPressThreshhold = 250;  //ms
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


  //Button pin declare
  /*  0: up
      1: down
      2: right
      3: left
  */
  buttons[0].buttonPin = 5;
  Serial.print("SET: ");
  Serial.println(buttons[0].buttonPin);
  buttons[1].buttonPin = 19;
  buttons[2].buttonPin = NULL;
  buttons[3].buttonPin = NULL;
}

unsigned short int pollBattery() {
  unsigned short int batteryLevelRaw;
  batteryLevelRaw = analogRead(batteryMonitorPin);

  return batteryLevelRaw;
}

void pushButtonState() {
  unsigned long pressDuration = 0;

  for (int i = 0; i < 3; i++) {
    // 0 is unpressed, 1 is pressed
    buttons[i].currentState = digitalRead(buttons[i].buttonPin);
    if (buttons[i].previousState == LOW && buttons[i].currentState == HIGH) {  //Button pressed
      buttons[i].timePressed = millis();  //Log pressed time
      buttons[i].samePress = true;

    } else if (buttons[i].previousState == HIGH && buttons[i].currentState == LOW) {  //Button released
      buttons[i].timeReleased = millis();                                             //Log released time
      buttons[i].samePress = false;

      pressDuration = buttons[i].timeReleased - buttons[i].timePressed;  //Pressed duration is these times subtracted

      if (pressDuration <= SHORT_PRESS_TIME) {
        buttons[i].pressStatus = "SHORT";
        //Serial.printf("[UPDATE] Updated button %d press status to SHORT", i);
      }

    } else if (buttons[i].previousState == HIGH && buttons[i].currentState == HIGH) {

      if (millis() - buttons[i].timePressed >= SHORT_PRESS_TIME) {
        buttons[i].pressStatus = "LONG";
      }
      
    } else {
      //If function hasn't initialized
      buttons[i].samePress = false;
      buttons[i].pressStatus = NULL;
      buttons[i].previousState = LOW;
    }
    buttons[i].previousState = buttons[i].currentState;
  }
}

bool rawPushButton() {
  return digitalRead(debugPushButtonPin);
}

void ioUpdate() {
  pushButtonState();
}