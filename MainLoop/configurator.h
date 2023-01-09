#ifndef _CONFIGURATOR
#define _CONFIGURATOR

#include <EEPROM.h>
#include <Preferences.h>

extern float batteryOffset;

void initializeConfigurator();
//initializes configurator

void configurator();
//main configurator loop

bool configureBatteryOffset();
//updates and refreshes battery values whenever a change is called for

void serialManagerBatteryConfig();
//reads serial monitor for any value inputs and calls configureBatterOffset() whenever the user inputs a battery voltage

float calculateTolerance();
//calculates the appropriate offset value for the battery reading
#endif
