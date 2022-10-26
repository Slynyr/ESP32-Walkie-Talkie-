//ExternalIO Declarations
#ifndef _EXTERNAL_IO_H_
#define _EXTERNAL_IO_H_
#include <Arduino.h>

void inputsInitialize();
//Initializes pins and pinmode

unsigned short int pollBattery();
//Polls the battery values for the display

bool debugPushbutton();
//Pushbutton demo

char* upButton();
//returns button interaction type for up button

#endif