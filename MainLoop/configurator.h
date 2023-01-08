#ifndef _CONFIGURATOR
#define _CONFIGURATOR

#include <EEPROM.h>

void configurator();
//main configurator loop

void configureBatteryOffset();
//updates and refreshes battery values whenever a change is called for

void serialManagerBatteryConfig();
//reads serial monitor for any value inputs and calls configureBatterOffset() whenever the user inputs a battery voltage

#endif
