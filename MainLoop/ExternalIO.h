//ExternalIO Declarations
#ifndef _EXTERNAL_IO_H_
#define _EXTERNAL_IO_H_
#include <Arduino.h>

#define SHORT_PRESS_TIME 500 // 500ms

void inputsInitialize();
//Initializes pins and pinmode

unsigned short int pollBattery();
//Polls the battery values for the display

void pushButtonState();
//Gives either "SHORT" or "LONG" depending on button press length

bool rawPushButton();
//Get raw state of pushbutton

void ioUpdate();
//Loop in mainloop to update IO

#endif