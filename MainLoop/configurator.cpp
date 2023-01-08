#include "UpdateDisplay.h"
#include "configurator.h"

//predefined memory address for battery offset and configurator status 
int isConfigCompleteAddr = 1000;
int batteryOffsetAddr = 1101; //tbd


bool ignoreConfig = false; //debug
bool isConfigComplete = EEPROM.get(isConfigCompleteAddr, isConfigComplete); 
float actualBatteryVoltage; 
float batteryOffset = EEPROM.get(batteryOffsetAddr, batteryOffset); 
float serialRead;

void configureBatteryOffset(){
    if (actualBatteryVoltage){
        Serial.printf("Wrote %d to memory\n", actualBatteryVoltage);
        EEPROM.put(isConfigCompleteAddr, true);
        EEPROM.put(batteryOffsetAddr, actualBatteryVoltage);
        EEPROM.get(isConfigCompleteAddr, isConfigComplete); 
        EEPROM.get(batteryOffsetAddr, batteryOffset); 
        Serial.printf("Current battery offset value in memory: %d\n", batteryOffset);
    }
}

void serialManagerBatteryConfig(){
    Serial.println("[CONFIG] Input current battery voltage into the serial monitor in order to complete setup process");
    drawText(true, 64, 32, 3, "Configure Battery", "WHITE");

    serialRead = Serial.parseFloat();
    if (serialRead != 0){
      actualBatteryVoltage = serialRead;
      Serial.printf("Updated battery offset to: %d\n", actualBatteryVoltage);
      configureBatteryOffset();
    }
}

void configurator(){
    if (!ignoreConfig || !isConfigComplete){
        serialManagerBatteryConfig();
    }
}
