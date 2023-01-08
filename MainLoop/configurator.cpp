#include "UpdateDisplay.h"
#include "configurator.h"

//predefined memory address for battery offset and configurator status 
int isConfigCompleteAddr = 1000;
int batteryOffsetAddr = 1101; //tbd


bool ignoreConfig = false; //debug
bool isConfigComplete = EEPROM.read(isConfigCompleteAddr); 
float actualBatteryVoltage; 
float batteryOffset = EEPROM.read(batteryOffsetAddr); 
float serialRead;

void configureBatteryOffset(){
    if (actualBatteryVoltage){
        Serial.printf("Wrote %d to memory\n", actualBatteryVoltage);
        EEPROM.write(isConfigCompleteAddr, true);
        EEPROM.write(batteryOffsetAddr, actualBatteryVoltage);
        isConfigComplete = EEPROM.read(isConfigCompleteAddr); 
        batteryOffset = EEPROM.read(batteryOffsetAddr); 
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
